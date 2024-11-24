#pragma once
#include <any>
#include <d3d12.h>
#include <memory>
#include <string>
#include <wrl/client.h>

namespace Lib::Graphics {
class Texture {
public:
    enum class Format : uint8_t {
        RGBA = 0,
        BGRA,
        Red,
    };

    static std::shared_ptr<Texture> create(const std::string& path);
    static std::shared_ptr<Texture> create(int32_t width, int32_t height, Format format, const uint8_t* data);
    ~Texture();

    Format getFormat() const;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const;

    static DXGI_FORMAT encodeFormat(Format format);
    static Format decodeFormat(DXGI_FORMAT format);
#endif

private:
    Texture();
    void init(int32_t width, int32_t height, Format format, const uint8_t* data);

    int32_t m_width;
    int32_t m_height;
    Format m_format;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
};
}