#include <Utils/String.hpp>
#include <Windows.h>
#include <stdexcept>

namespace Lib::Utils {
namespace String {

    std::wstring toWideString(const std::string& str)
    {
        // see: https://stackoverflow.com/questions/42946335/deprecated-header-codecvt-replacement
        if (str.empty()) {
            return L"";
        }

        const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int32_t>(str.size()), nullptr, 0);
        if (size_needed <= 0) {
            throw std::runtime_error("MultiByteToWideChar() failed: " + std::to_string(size_needed));
        }

        std::wstring result(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int32_t>(str.size()), result.data(), size_needed);
        return result;
    }

    std::string toMultiByteString(const std::wstring& str)
    {
        // see: https://stackoverflow.com/questions/42946335/deprecated-header-codecvt-replacement
        if (str.empty()) {
            return "";
        }

        const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int32_t>(str.size()), nullptr, 0, nullptr, nullptr);
        if (size_needed <= 0) {
            throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
        }

        std::string result(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int32_t>(str.size()), result.data(), size_needed, nullptr, nullptr);
        return result;
    }
}
}