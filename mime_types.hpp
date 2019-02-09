//
// Created by SashiRin on 2019-02-08.
//

#ifndef STHS_MIME_TYPES_HPP
#define STHS_MIME_TYPES_HPP

#include <string>
#include <vector>

namespace sashi_tiny_http {
    using std::string;
    using std::vector;
    namespace mime_types {

        /// Convert a file extension into a MIME type.
        string ConvertExtensionToType(const string &extension);
    }
}

#endif //STHS_MIME_TYPES_HPP
