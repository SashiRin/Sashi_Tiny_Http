//
// Created by SashiRin on 2019-02-08.
//

#ifndef SASHI_TINY_HTTP_UTILITY_HPP
#define SASHI_TINY_HTTP_UTILITY_HPP

#include <unordered_map>
#include <string>
#include <sstream>

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

    static bool CheckIfChar(int c) {
        return (c >= 0 && c <= 127);
    }

    static bool CheckIfCtl(int c) {
        return ((c >= 0 && c <= 31) || c == 127);
    }

    static bool CheckIfTspecial(int c) {
        switch (c) {
            case '(':
            case ')':
            case '<':
            case '>':
            case '@':
            case ',':
            case ';':
            case ':':
            case '\\':
            case '"':
            case '/':
            case '[':
            case ']':
            case '?':
            case '=':
            case '{':
            case '}':
            case ' ':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    static bool CheckIfDigit(int c) {
        return (c >= '0' && c <= '9');
    }

    struct HeaderItem {
        string name;
        string value;
        void Clear() {
            this->name.clear();
            this->value.clear();
        }
    };

    static bool DecodeUrl(const std::string &in, std::string &out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.length(); ++i) {
            if (in[i] == '%') {
                if (i + 3 <= in.length()) {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out.push_back(static_cast<char>(value));
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out.push_back(' ');
            } else {
                out.push_back(in[i]);
            }
        }
        return true;
    }

    static std::string GetPathExtension(const std::string &path) {
        auto last_slash_pos = path.find_last_of('/');
        auto last_dot_pos = path.find_last_of('.');
        std::string extension;
        if (last_slash_pos != std::string::npos && last_dot_pos > last_slash_pos) {
            extension = path.substr(last_dot_pos + 1);
        }
        return extension;
    }

    namespace misc_strings {
        const char name_value_separator[] = {':', ' '};
        const char crlf[] = {'\r', '\n'};
    }
}

#endif //SASHI_TINY_HTTP_UTILITY_HPP
