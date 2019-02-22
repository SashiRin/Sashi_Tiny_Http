//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_REQUEST_HPP
#define SASHI_TINY_HTTP_REQUEST_HPP

#include <string>
#include <tuple>
#include <map>
#include "utility.hpp"
#include "status_code.hpp"
#include "response.hpp"

namespace sashi_tiny_http {
    using std::string;
    struct Request {
        string method;
        string uri;
        int http_version_major = 0;
        int http_version_minor = 0;
        CaseInsensitiveMultimap headers;
    };

    class RequestParser {
    public:
        RequestParser();

        enum class ResultType {
            kGood,
            kBad,
            kIndeterminate
        };

        template<class InputIterator>
        std::tuple<ResultType, InputIterator> Parse(Request &request, InputIterator begin, InputIterator end) {
            while (begin != end) {
                ResultType result = Consume(request, *begin++);
                if (result == ResultType::kGood || result == ResultType::kBad) {
                    return std::make_tuple(result, begin);
                }
            }
            return std::make_tuple(ResultType::kIndeterminate, begin);
        }

    private:

        ResultType Consume(Request &request, char input);

        enum state {
            kMethodStart,
            kMethod,
            kUri,
            kHttpVersionH,
            kHttpVersionT1,
            kHttpVersionT2,
            kHttpVersionP,
            kHttpVersionSlash,
            kHttpVersionMajorStart,
            kHttpVersionMajor,
            kHttpVersionMinorStart,
            kHttpVersionMinor,
            kExpectingNewline1,
            kHeaderLineStart,
            kHeaderLws,
            kHeaderName,
            kSpaceBeforeHeaderValue,
            kHeaderValue,
            kExpectingNewline2,
            kExpectingNewline3
        } state_;

        HeaderItem header_item;

    };

    class RequestHandler {
    public:
        RequestHandler(const RequestHandler &) = delete;

        RequestHandler &operator=(const RequestHandler &) = delete;

        explicit RequestHandler(boost::asio::io_context &io, const std::string &doc_root);

        void HandleRequest(const Request &request, Response &response);

        void StopFileCache();

    private:
        std::string doc_root_;

        ExpireMap<string, string> file_cache_;
    };
}

#endif //SASHI_TINY_HTTP_REQUEST_HPP
