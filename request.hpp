//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_REQUEST_HPP
#define SASHI_TINY_HTTP_REQUEST_HPP

#include <string>
#include "utility.hpp"
#include "status_code.hpp"

namespace sashi_tiny_http {
    using std::string;
    struct HttpRequest {
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
        std::tuple<ResultType, InputIterator> Parse(HttpRequest &request, InputIterator begin, InputIterator end) {

            while (begin != end) {
                ResultType result = Consume(request, *begin++);
                if (result == ResultType::kGood || result == ResultType::kBad) {
                    return std::make_tuple(result, begin);
                }
            }
        }

    private:

        ResultType Consume(HttpRequest &request, char input);

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

        struct HeaderItem {
            string name;
            string value;
            void Clear() {
                this->name.clear();
                this->value.clear();
            }
        } header_item;

    };
}

#endif //SASHI_TINY_HTTP_REQUEST_HPP
