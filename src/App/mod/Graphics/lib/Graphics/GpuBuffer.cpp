#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/GpuBuffer.hpp>
#include <cassert>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<GpuBuffer> GpuBuffer::create()
{
    auto gpuBuffer = std::shared_ptr<GpuBuffer>(new GpuBuffer());
    return gpuBuffer;
}

GpuBuffer::~GpuBuffer()
{
}

void GpuBuffer::allocate(size_t size)
{
    assert(size > 0);
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = size;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = DXGI_FORMAT_UNKNOWN;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_COMMON;
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

size_t GpuBuffer::getSize() const
{
    return m_size;
}
// internal
void GpuBuffer::stateUAV(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_resource.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    cmdList->ResourceBarrier(1, &barrier);
}

void GpuBuffer::stateCommon(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_resource.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    cmdList->ResourceBarrier(1, &barrier);
}

void GpuBuffer::stateSync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrier.UAV.pResource = m_resource.Get();

    cmdList->ResourceBarrier(1, &barrier);
}

Microsoft::WRL::ComPtr<ID3D12Resource> GpuBuffer::getID3D12Resource() const
{
    return m_resource;
}
// private
GpuBuffer::GpuBuffer()
    : m_size(0)
    , m_resource()
{
}
}