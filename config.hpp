//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_CONFIG_HPP
#define SASHI_TINY_HTTP_CONFIG_HPP

#include <limits>

namespace sashi_tiny_http {

    namespace config {

        static std::string address;

        static unsigned short port;

        static long timeout_request = 5;

        static long timeout_content = 300;

        static std::size_t max_request_streambuf_size = std::numeric_limits<std::size_t>::max();

        // max cache file size, default 20MB
        static size_t max_cache_file_size = 20 * 1LL << 20;

    }
}

#endif //SASHI_TINY_HTTP_CONFIG_HPP
