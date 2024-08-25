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
    static std::shared_ptr<Texture> create(const std::wstring& path);
    static std::shared_ptr<Texture> create(int32_t width, int32_t height, const uint8_t* data);
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
#endif
};
}