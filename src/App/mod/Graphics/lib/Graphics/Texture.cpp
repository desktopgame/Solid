#include <DirectXTex.h>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Texture.hpp>
#include <cassert>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<Texture> Texture::create(const std::wstring& path)
{
    DirectX::TexMetadata md;
    DirectX::ScratchImage image;
    if (FAILED(DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &md, image))) {
        return nullptr;
    }
    int32_t width = static_cast<int32_t>(md.width);
    int32_t height = static_cast<int32_t>(md.height);
    assert(md.format == DXGI_FORMAT_B8G8R8A8_UNORM);
    return create(width, height, image.GetPixels());
}

std::shared_ptr<Texture> Texture::create(int32_t width, int32_t height, const uint8_t* data)
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    auto texture = std::shared_ptr<Texture>(new Texture());
    texture->m_width = width;
    texture->m_height = height;

    D3D12_HEAP_PROPERTIES texHeapProps = {};
    texHeapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    texHeapProps.CreationNodeMask = 0;
    texHeapProps.VisibleNodeMask = 0;
    D3D12_RESOURCE_DESC texResDesc = {};
    texResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texResDesc.Width = width;
    texResDesc.Height = height;
    texResDesc.DepthOrArraySize = 1;
    texResDesc.SampleDesc.Count = 1;
    texResDesc.SampleDesc.Quality = 0;
    texResDesc.MipLevels = 1;
    texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    if (FAILED(device->CreateCommittedResource(&texHeapProps, D3D12_HEAP_FLAG_NONE, &texResDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&texture->m_resource)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    if (FAILED(texture->m_resource->WriteToSubresource(0, nullptr, data, sizeof(uint8_t) * 4 * width, sizeof(uint8_t) * 4 * (width * height)))) {
        throw std::runtime_error("failed WriteToSubresource()");
    }
    return texture;
}

Texture::~Texture()
{
}
// internal
Microsoft::WRL::ComPtr<ID3D12Resource> Texture::getID3D12Resource() const
{
    return m_resource;
}
// private
Texture::Texture()
    : m_width(0)
    , m_height(0)
    , m_resource()
{
}
}