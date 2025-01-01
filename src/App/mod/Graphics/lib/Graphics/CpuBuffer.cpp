#include <Graphics/CpuBuffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/GpuBuffer.hpp>
#include <cassert>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<CpuBuffer> CpuBuffer::create()
{
    auto cpuBuffer = std::shared_ptr<CpuBuffer>(new CpuBuffer());
    return cpuBuffer;
}

CpuBuffer::~CpuBuffer()
{
}

void CpuBuffer::allocate(size_t size)
{
    assert(size > 0);
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = size;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = DXGI_FORMAT_UNKNOWN;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_GENERIC_READ;
    if (FAILED(device->CreateCommittedResource(
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

void CpuBuffer::update(const void* data)
{
    void* outData;
    if (FAILED(m_resource->Map(0, nullptr, (void**)&outData))) {
        throw std::runtime_error("failed Map()");
    }
    ::memcpy(outData, data, m_size);
    m_resource->Unmap(0, nullptr);
    m_version++;
}

size_t CpuBuffer::getSize() const
{
    return m_size;
}
int32_t CpuBuffer::getVersion() const { return m_version; }
// internal
void CpuBuffer::transport(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<GpuBuffer>& dst)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = dst->getID3D12Resource().Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    cmdList->ResourceBarrier(1, &barrier);
    cmdList->CopyResource(dst->getID3D12Resource().Get(), m_resource.Get());

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
    cmdList->ResourceBarrier(1, &barrier);
}

Microsoft::WRL::ComPtr<ID3D12Resource> CpuBuffer::getID3D12Resource() const
{
    return m_resource;
}
// private
CpuBuffer::CpuBuffer()
    : m_size(0)
    , m_version()
    , m_resource()
{
}
}