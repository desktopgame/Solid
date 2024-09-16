#pragma once
#include <string>
#include <unordered_map>

namespace Lib::Utils {
namespace String {

    std::wstring toWideString(const std::string& str);

    std::string toMultiByteString(const std::wstring& str);

    template <typename T>
    inline std::basic_string<T> interpolate(std::basic_istream<T>& is, const std::unordered_map<std::basic_string<T>, std::basic_string<T>>& map)
    {
        using char_type = typename std::basic_istream<T>::traits_type::char_type;
        using int_type = typename std::istream::traits_type::int_type;
        std::basic_stringstream<T> buf;
        std::vector<T> tmp;
        const char_type k_chD = static_cast<char_type>('$');
        const char_type k_chL = static_cast<char_type>('{');
        const char_type k_chR = static_cast<char_type>('}');
        const char_type k_chNone = static_cast<char_type>('\n');
        const char_type k_chUs = static_cast<char_type>('_');
        const char_type k_chCo = static_cast<char_type>(':');
        char_type chStatus = k_chNone;
        while (!is.eof()) {
            int_type iCode = is.get();
            char_type ch = static_cast<char_type>(iCode);
            // end of stream.
            if (iCode == EOF) {
                break;
            }
            if (chStatus == k_chD) {
                // ${
                if (ch == k_chL) {
                    chStatus = ch;
                } else if (ch == k_chD) {
                    // $$
                    buf << '$';
                } else {
                    // $
                    buf << '$' << ch;
                    chStatus = k_chNone;
                }
            } else if (chStatus == k_chL) {
                // ${...}
                if (ch == k_chR) {
                    // ... -> map[...]
                    std::basic_string<T> str(tmp.begin(), tmp.end());
                    if (map.count(str)) {
                        str = map.at(str);
                    }
                    for (char_type c : str) {
                        buf << c;
                    }
                    chStatus = k_chNone;
                    tmp.clear();
                } else {
                    // ${...
                    if (!std::isalnum(iCode) && ch != k_chUs && ch != k_chCo) {
                        throw std::invalid_argument("invalid format.");
                    }
                    tmp.emplace_back(ch);
                }
            } else {
                if (ch == k_chD) {
                    // $
                    chStatus = ch;
                } else {
                    buf << ch;
                }
            }
        }
        return buf.str();
    }

    template <typename T>
    inline std::basic_string<T> interpolate(const std::basic_string<T>& input, const std::unordered_map<std::basic_string<T>, std::basic_string<T>>& map)
    {
        std::basic_stringbuf<T> buf(input.c_str());
        std::istream is(&buf);
        return interpolate<T>(is, map);
    }

}
}