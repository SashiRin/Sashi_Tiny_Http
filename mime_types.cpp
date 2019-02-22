//
// Created by SashiRin on 2019-02-08.
//

#include "mime_types.hpp"
#include <vector>

namespace sashi_tiny_http {
    namespace mime_types {
        struct MappingItem {
            const string extension;
            const string mime_type;
        };

        vector<MappingItem> mapping_table = {
            {"html", "text/html"},
            {"htm", "text/html"},
            {"php", "text/html"},
            {"css", "text/css"},
            {"txt", "text/plain"},
            {"js", "application/javascript"},
            {"json", "application/json"},
            {"xml", "application/xml"},
            {"bmp", "image/bmp"},
            {"gif", "image/gif"},
            {"jpg", "image/jpeg"},
            {"jpe", "image/jpeg"},
            {"jpeg", "image/jpeg"},
            {"png", "image/png"},
            {"svg", "image/svg+xml"},
            {"svgz", "image/svg+xml"},
            {"tiff", "image/tiff"},
            {"tif", "image/tiff"},
            {"ico", "vnd.microsoft.icon"},
            {"swf", "application/x-shockwave-flash"},
            {"flv", "video/x-flv"},
        };

        string ConvertExtensionToType(const string &extension) {
            for (auto &m : mapping_table) {
                if (m.extension == extension) {
                    return m.mime_type;
                }
            }
            return "application/text";
        }
    }
}