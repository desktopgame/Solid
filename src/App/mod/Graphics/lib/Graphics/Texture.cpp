#include <DirectXTex.h>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Texture.hpp>
#include <Utils/String.hpp>
#include <cassert>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<Texture> Texture::create(const std::string& path)
{
    DirectX::TexMetadata md;
    DirectX::ScratchImage image;
    if (FAILED(DirectX::LoadFromWICFile(Utils::String::toWideString(path).c_str(), DirectX::WIC_FLAGS_NONE, &md, image))) {
        return nullptr;
    }
    int32_t width = static_cast<int32_t>(md.width);
    int32_t height = static_cast<int32_t>(md.height);
    return create(width, height, decodeFormat(md.format), image.GetPixels());
}

std::shared_ptr<Texture> Texture::create(int32_t width, int32_t height, Format format, const uint8_t* data)
{
    auto texture = std::shared_ptr<Texture>(new Texture());
    texture->init(width, height, format, data);
    return texture;
}

Texture::~Texture()
{
}

Texture::Format Texture::getFormat() const { return m_format; }
// internal
Microsoft::WRL::ComPtr<ID3D12Resource> Texture::getID3D12Resource() const
{
    return m_resource;
}

DXGI_FORMAT Texture::encodeFormat(Format format)
{
    switch (format) {
    case Format::Red:
        return DXGI_FORMAT_R8_UNORM;
    case Format::RGBA:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case Format::BGRA:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    }
}

Texture::Format Texture::decodeFormat(DXGI_FORMAT format)
{
    switch (format) {
    case DXGI_FORMAT_R8_UNORM:
        return Format::Red;
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return Format::RGBA;
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        return Format::BGRA;
    default:
        throw std::logic_error("unsupported format.");
    }
}
// private
Texture::Texture()
    : m_width(0)
    , m_height(0)
    , m_format(Format::RGBA)
    , m_resource()
{
}

void Texture::init(int32_t width, int32_t height, Format format, const uint8_t* data)
{
    m_width = width;
    m_height = height;
    m_format = format;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_HEAP_PROPERTIES texHeapProps = {};
    texHeapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    texHeapProps.CreationNodeMask = 0;
    texHeapProps.VisibleNodeMask = 0;
    D3D12_RESOURCE_DESC texResDesc = {};
    texResDesc.Format = encodeFormat(format);
    texResDesc.Width = width;
    texResDesc.Height = height;
    texResDesc.DepthOrArraySize = 1;
    texResDesc.SampleDesc.Count = 1;
    texResDesc.SampleDesc.Quality = 0;
    texResDesc.MipLevels = 1;
    texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    if (FAILED(device->CreateCommittedResource(&texHeapProps, D3D12_HEAP_FLAG_NONE, &texResDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&m_resource)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    uint32_t rowSize = 0;
    uint32_t totalSize = 0;
    switch (format) {
    case Format::Red:
        rowSize = sizeof(uint8_t) * width;
        totalSize = sizeof(uint8_t) * (width * height);
        break;
    case Format::RGBA:
    case Format::BGRA:
        rowSize = sizeof(uint8_t) * 4 * width;
        totalSize = sizeof(uint8_t) * 4 * (width * height);
        break;
    }
    if (FAILED(m_resource->WriteToSubresource(0, nullptr, data, rowSize, totalSize))) {
        throw std::runtime_error("failed WriteToSubresource()");
    }
}
}