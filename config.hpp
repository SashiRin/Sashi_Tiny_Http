//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_CONFIG_HPP
#define SASHI_TINY_HTTP_CONFIG_HPP

namespace sashi_tiny_http {

    class Config {

        Config(unsigned short _port) : port(_port) {}

    public:

        std::string address;

        unsigned short port;

        long timeout_request = 5;

        long timeout_content = 300;

        std::size_t max_request_streambuf_size = std::numeric_limits<std::size_t>::max();

    };
}

#endif //SASHI_TINY_HTTP_CONFIG_HPP
