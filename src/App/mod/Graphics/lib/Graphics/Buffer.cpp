#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <cassert>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<Buffer> Buffer::create(Type type)
{
    auto buffer = std::shared_ptr<Buffer>(new Buffer());
    buffer->m_type = type;
    buffer->m_device = Engine::getInstance()->getDevice()->getID3D12Device();
    return buffer;
}

Buffer::~Buffer()
{
}

void Buffer::allocate(size_t size)
{
    assert(size > 0);
    bool isReadWriteBuffer = m_type == Type::ReadWrite;
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
    resDesc.Flags = isReadWriteBuffer ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : D3D12_RESOURCE_FLAG_NONE;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_RESOURCE_STATES initState = {};
    switch (m_type) {
    case Type::Vertex:
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        initState = D3D12_RESOURCE_STATE_GENERIC_READ;
        break;
    case Type::ReadWrite:
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        initState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
        break;
    }
    if (FAILED(m_device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            initState,
            nullptr,
            IID_PPV_ARGS(&m_resource)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    m_size = size;
}

void Buffer::update(const void* data)
{
    void* outData;
    if (FAILED(m_resource->Map(0, nullptr, (void**)&outData))) {
        throw std::runtime_error("failed Map()");
    }
    ::memcpy(outData, data, m_size);
    m_resource->Unmap(0, nullptr);
}

size_t Buffer::getSize() const
{
    return m_size;
}
// internal
void Buffer::transport(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<Buffer>& dst)
{
    if (m_type != Type::Vertex || dst->m_type != Type::ReadWrite) {
        throw std::logic_error("failed CopyResource()");
    }
    cmdList->CopyResource(dst->m_resource.Get(), m_resource.Get());
}

Microsoft::WRL::ComPtr<ID3D12Resource> Buffer::getID3D12Resource() const
{
    return m_resource;
}
// private
Buffer::Buffer()
    : m_size(0)
    , m_resource()
{
}
}