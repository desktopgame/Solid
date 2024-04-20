#pragma once
#include <any>
#include <memory>
#include <string>

namespace Lib::Graphics {
class Texture {
public:
    static std::shared_ptr<Texture> create(const std::wstring& path);
    static std::shared_ptr<Texture> create(int32_t width, int32_t height, const uint8_t* data);
    ~Texture();

    std::any getHandle() const;

private:
    Texture();

    int32_t m_width;
    int32_t m_height;

    class Impl;
    std::shared_ptr<Impl> m_impl;
};
}