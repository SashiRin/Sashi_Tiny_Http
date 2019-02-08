//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_HEADER_HPP
#define SASHI_TINY_HTTP_HEADER_HPP

namespace sashi_tiny_http {
    namespace server {

        struct HttpHeader {
            string name;
            string value;
        };
    }
}

#endif //SASHI_TINY_HTTP_HEADER_HPP
