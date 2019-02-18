//
// Created by SashiRin on 2019-02-09.
//

#include "response.hpp"
#include <boost/format.hpp>

namespace sashi_tiny_http {

    std::vector<boost::asio::const_buffer> Response::ConvertToBuffers() {
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(status_line));
        for (auto &header : headers) {
            buffers.push_back(boost::asio::buffer(header.name));
            buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
            buffers.push_back(boost::asio::buffer(header.value));
            buffers.push_back(boost::asio::buffer(misc_strings::crlf));
        }
        buffers.push_back(boost::asio::buffer(misc_strings::crlf));
        buffers.push_back(boost::asio::buffer(content));
        return buffers;
    }

    Response Response::StockReply(StatusCode status) {
        Response response;
        response.status = status;
        response.status_line = "HTTP/1.0 " + status_code_strings.at(status) + misc_strings::crlf;
        response.content = boost::str(boost::format(
            "<html>"
            "<head><title>%1%</title></head>"
            "<body><h1>%2%</h1></body>"
            "</html>") % status_code_strings.at(status) % status_code_strings.at(status));
        response.headers.resize(3);
        response.headers[0].name = "Content-Length";
        response.headers[0].value = std::to_string(response.content.length());
        response.headers[1].name = "Content-Type";
        response.headers[1].value = "text/html";
        response.headers[2].name = "Server";
        response.headers[2].value = "SashiTinyHttp";
        return response;
    }
}
