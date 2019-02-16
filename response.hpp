//
// Created by SashiRin on 2019-02-09.
//

#ifndef SASHI_TINY_HTTP_RESPONSE_HPP
#define SASHI_TINY_HTTP_RESPONSE_HPP

#include <vector>
#include <string>
#include <boost/asio.hpp>
#include "utility.hpp"
#include "status_code.hpp"

namespace sashi_tiny_http {

    class Response {
    public:
        StatusCode status = StatusCode::unknown;

        std::string status_line;

        std::vector<HeaderItem> headers;

        std::string content;

        std::vector<boost::asio::const_buffer> ConvertToBuffers();

        static Response StockReply(StatusCode status);

    };

}

#endif //SASHI_TINY_HTTP_RESPONSE_HPP
