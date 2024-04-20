#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// Impl
class Buffer::Impl {
public:
    explicit Impl() = default;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12Resource> resource;
};
// public
std::shared_ptr<Buffer> Buffer::create()
{
    auto buffer = std::shared_ptr<Buffer>(new Buffer());
    buffer->m_impl->device = std::any_cast<ComPtr<ID3D12Device>>(Engine::getInstance()->getDevice()->getHandle());
    return buffer;
}

Buffer::~Buffer()
{
}

void Buffer::allocate(size_t size)
{
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = size;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = DXGI_FORMAT_UNKNOWN;
    resDesc.SampleDesc.Count = 1;
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    if (FAILED(m_impl->device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_impl->resource)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    m_size = size;
}

void Buffer::update(const void* data)
{
    void* outData;
    if (FAILED(m_impl->resource->Map(0, nullptr, (void**)&outData))) {
        throw std::runtime_error("failed Map()");
    }
    ::memcpy(outData, data, m_size);
    m_impl->resource->Unmap(0, nullptr);
}

uint64_t Buffer::getVirtualAddress() const
{
    return static_cast<uint64_t>(m_impl->resource->GetGPUVirtualAddress());
}
size_t Buffer::getSize() const
{
    return m_size;
}
// private
Buffer::Buffer()
    : m_size(0)
    , m_impl(std::make_shared<Impl>())
{
}
}