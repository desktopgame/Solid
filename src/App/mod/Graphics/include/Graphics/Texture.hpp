#pragma once
#include <any>
#include <memory>
#include <string>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Texture {
public:
    enum class Format : uint8_t {
        RGBA = 0,
        Red,
    };

    static std::shared_ptr<Texture> create(const std::wstring& path);
    static std::shared_ptr<Texture> create(int32_t width, int32_t height, Format format, const uint8_t* data);
    ~Texture();

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const;
#endif

private:
    Texture();

    int32_t m_width;
    int32_t m_height;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;

    static DXGI_FORMAT toPrivateFormat(Format format);
    static Format toPublicFormat(DXGI_FORMAT format);

#endif
};
}