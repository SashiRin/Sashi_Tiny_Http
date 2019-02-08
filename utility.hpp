//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_UTILITY_HPP
#define SASHI_TINY_HTTP_UTILITY_HPP

#include <unordered_map>
#include <string>

namespace sashi_tiny_http {
    using std::size_t;
    using std::string;
    inline bool CheckCaseInsensitiveEqual(const string &str1, const string &str2) {
        return str1.length() == str2.length() && std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
            return tolower(a) == tolower(b);
        });
    }

    class CaseInsensitiveEqual {
    public:
        bool operator() (const string &str1, const string &str2) const {
            return CheckCaseInsensitiveEqual(str1, str2);
        }
    };

    // Based on https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x/2595226#2595226
    class CaseInsensitiveHash {
    public:
        size_t operator() (const string &str) const {
            size_t h = 0;
            std::hash<int> hash;
            for (auto &c : str) {
                h ^= hash(tolower(c)) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };

    using CaseInsensitiveMultimap = std::unordered_multimap<string, string, CaseInsensitiveHash, CaseInsensitiveEqual>;
}

#endif //SASHI_TINY_HTTP_UTILITY_HPP
