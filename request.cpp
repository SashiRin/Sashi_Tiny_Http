//
// Created by SashiRin on 2019-02-09.
//

#include "request.hpp"
#include "mime_types.hpp"
#include "config.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace sashi_tiny_http {
    RequestParser::RequestParser() : state_(kMethodStart) {}

    RequestParser::ResultType RequestParser::Consume(Request &request, char input) {
//        std::cout << input;
        switch (state_) {
            case kMethodStart:
                if (!CheckIfChar(input) || CheckIfCtl(input) || CheckIfTspecial(input)) {
                    return ResultType::kBad;
                } else {
                    request.method.push_back(input);
                    state_ = kMethod;
                    return ResultType::kIndeterminate;
                }
            case kMethod:
                if (input == ' ') {
                    state_ = kUri;
                    return ResultType::kIndeterminate;
                } else if (!CheckIfChar(input) || CheckIfCtl(input) || CheckIfTspecial(input)) {
                    return ResultType::kBad;
                } else {
                    request.method.push_back(input);
                    return ResultType::kIndeterminate;
                }
            case kUri:
                if (input == ' ') {
                    state_ = kHttpVersionH;
                    return ResultType::kIndeterminate;
                } else if (CheckIfCtl(input)) {
                    return ResultType::kBad;
                } else {
                    request.uri.push_back(input);
                    return ResultType::kIndeterminate;
                }
            case kHttpVersionH:
                if (input == 'H') {
                    state_ = kHttpVersionT1;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHttpVersionT1:
                if (input == 'T') {
                    state_ = kHttpVersionT2;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHttpVersionT2:
                if (input == 'T') {
                    state_ = kHttpVersionP;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHttpVersionP:
                if (input == 'P') {
                    state_ = kHttpVersionSlash;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHttpVersionSlash:
                if (input == '/') {
                    state_ = kHttpVersionMajor;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHttpVersionMajor:
                if (input == '.') {
                    state_ = kHttpVersionMinor;
                    return ResultType::kIndeterminate;
                } else if (CheckIfDigit(input)) {
                    request.http_version_major = input - '0';
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHttpVersionMinor:
                if (input == '\r') {
                    state_ = kExpectingNewline1;
                    return ResultType::kIndeterminate;
                } else if (CheckIfDigit(input)) {
                    request.http_version_minor = input - '0';
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kExpectingNewline1:
                if (input == '\n') {
                    state_ = kHeaderLineStart;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHeaderLineStart:
                if (input == '\r') {
                    state_ = kExpectingNewline3;
                    return ResultType::kIndeterminate;
                } else if (!request.headers.empty() && (input == ' ' || input == '\t')) {
                    state_ = kHeaderLws;
                    return ResultType::kIndeterminate;
                } else if (!CheckIfChar(input) || CheckIfCtl(input) || CheckIfTspecial(input)) {
                    return ResultType::kBad;
                } else {
                    header_item.Clear();
                    header_item.name.push_back(input);
                    state_ = kHeaderName;
                    return ResultType::kIndeterminate;
                }
            case kHeaderLws:
                if (input == '\r') {
                    state_ = kExpectingNewline2;
                    return ResultType::kIndeterminate;
                } else if (input == ' ' || input == '\t') {
                    return ResultType::kIndeterminate;
                } else if (CheckIfCtl(input)) {
                    return ResultType::kBad;
                } else {
                    header_item.value.push_back(input);
                    state_ = kHeaderValue;
                    return ResultType::kIndeterminate;
                }
            case kHeaderName:
                if (input == ':') {
                    state_ = kSpaceBeforeHeaderValue;
                    return ResultType::kIndeterminate;
                } else if (!CheckIfChar(input) || CheckIfCtl(input) || CheckIfTspecial(input)) {
                    return ResultType::kBad;
                } else {
                    header_item.name.push_back(input);
                    return ResultType::kIndeterminate;
                }
            case kSpaceBeforeHeaderValue:
                if (input == ' ') {
                    state_ = kHeaderValue;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kHeaderValue:
                if (input == '\r') {
                    request.headers.insert(std::make_pair(header_item.name, header_item.value));
                    state_ = kExpectingNewline2;
                    return ResultType::kIndeterminate;
                } else if (CheckIfCtl(input)) {
                    return ResultType::kBad;
                } else {
                    header_item.value.push_back(input);
                    return ResultType::kIndeterminate;
                }
            case kExpectingNewline2:
                if (input == '\n') {
                    state_ = kHeaderLineStart;
                    return ResultType::kIndeterminate;
                } else {
                    return ResultType::kBad;
                }
            case kExpectingNewline3:
                return (input == '\n') ? ResultType::kGood : ResultType::kBad;
            default:
                return ResultType::kBad;
        }
    }

    RequestHandler::RequestHandler(boost::asio::io_context &io, const std::string &doc_root) : file_cache_(io, config::cache_file_duration), doc_root_(doc_root) {}

    void RequestHandler::HandleRequest(const Request &request, Response &response) {
        // Decode url to path
        std::string request_path;
        if (!DecodeUrl(request.uri, request_path)) {
            response = Response::StockReply(StatusCode::client_error_bad_request);
            return;
        }

        // Request path must be absolute and not contain ".."
        if (request_path.empty() || request_path.front() != '/' || request_path.find("..") != std::string::npos) {
            response = Response::StockReply(StatusCode::client_error_bad_request);
            return;
        }

        if (request_path.back() == '/') {
            request_path += "index.html";
        }

        // Determine the file extension
        std::string extension = GetPathExtension(request_path);

        // If file is in the cache, directly return it
        string request_value;
        if (file_cache_.get(request_path, request_value)) {
            response.status = StatusCode::success_ok;
            response.status_line = "HTTP/1.1 " + status_code_strings.at(response.status) + misc_strings::crlf;
            response.content = request_value;
        } else {
            // Open the file to send back
            std::string full_path = doc_root_ + request_path;
            std::ifstream ifs(full_path.c_str(), std::ios::in | std::ios::binary);
            if (!ifs) {
                response = Response::StockReply(StatusCode::client_error_not_found);
                return;
            }

            // Fill out the response to be sent to the client
            response.status = StatusCode::success_ok;
            response.status_line = "HTTP/1.1 " + status_code_strings.at(response.status) + misc_strings::crlf;
            response.content.clear();
            char buffer[512];
            while (ifs.read(buffer, sizeof(buffer)).gcount() > 0) {
                response.content.append(buffer, ifs.gcount());
            }
            // Add to the cache
            if (response.content.length() <= config::max_cache_file_size) {
                file_cache_.insert(request_path, response.content);
            }
        }

        response.headers.resize(5);
        response.headers[0].name = "Content-Length";
        response.headers[0].value = std::to_string(response.content.length());
        response.headers[1].name = "Content-Type";
        response.headers[1].value = mime_types::ConvertExtensionToType(extension);
        response.headers[2].name = "Connection";
        response.headers[2].value = "keep-alive";
        response.headers[3].name = "Cache-Control";
        response.headers[3].value = "max-age=86400";
        response.headers[4].name = "Server";
        response.headers[4].value = "SashiTinyHttp";
    }

    void RequestHandler::StopFileCache() {
        file_cache_.stop();
    }
}

