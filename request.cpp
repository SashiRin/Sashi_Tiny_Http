//
// Created by SashiRin on 2019-02-09.
//

#include "request.hpp"

namespace sashi_tiny_http {
    RequestParser::RequestParser() : state_(kMethodStart) {}

    RequestParser::ResultType RequestParser::Consume(HttpRequest &request, char input) {
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
}

