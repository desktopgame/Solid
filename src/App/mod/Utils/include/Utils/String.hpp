#pragma once
#include <string>

namespace Lib::Utils {
namespace String {

    std::wstring toWideString(const std::string& str);

    std::string toMultiByteString(const std::wstring& str);
}
}