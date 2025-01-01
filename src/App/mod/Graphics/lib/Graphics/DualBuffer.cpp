#include <Graphics/CpuBuffer.hpp>
#include <Graphics/DualBuffer.hpp>
#include <Graphics/GpuBuffer.hpp>

namespace Lib::Graphics {
// public
std::shared_ptr<DualBuffer> DualBuffer::create()
{
    auto dualBuffer = std::shared_ptr<DualBuffer>(new DualBuffer());
    return dualBuffer;
}

void DualBuffer::allocate(size_t size)
{
    m_cpuBuffer = CpuBuffer::create();
    m_gpuBuffer = GpuBuffer::create();

    m_cpuBuffer->allocate(size);
}

void DualBuffer::update(const void* data)
{
    m_cpuBuffer->update(data);
}

void DualBuffer::reset(const std::shared_ptr<CpuBuffer>& cpuBuffer, const std::shared_ptr<GpuBuffer>& gpuBuffer)
{
    m_version = 0;
    m_cpuBuffer = cpuBuffer;
    m_gpuBuffer = gpuBuffer;
}

std::shared_ptr<CpuBuffer> DualBuffer::getCpuBuffer() const { return m_cpuBuffer; }
std::shared_ptr<GpuBuffer> DualBuffer::getGpuBuffer() const { return m_gpuBuffer; }
size_t DualBuffer::getSize() const
{
    if (!m_gpuBuffer) {
        return 0;
    }
    return m_gpuBuffer->getSize();
}

void DualBuffer::sync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    if (m_version == 0 || m_version != m_cpuBuffer->getVersion()) {
        m_cpuBuffer->transport(cmdList, m_gpuBuffer);
    }
    m_version = m_cpuBuffer->getVersion();
}

Microsoft::WRL::ComPtr<ID3D12Resource> DualBuffer::getID3D12Resource() const
{
    if (!m_gpuBuffer) {
        return nullptr;
    }
    return m_gpuBuffer->getID3D12Resource();
}
// private
DualBuffer::DualBuffer()
    : m_version()
    , m_cpuBuffer()
    , m_gpuBuffer()
{
}
}