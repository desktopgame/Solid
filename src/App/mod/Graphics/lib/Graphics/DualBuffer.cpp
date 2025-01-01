#include <Graphics/CpuBuffer.hpp>
#include <Graphics/DualBuffer.hpp>
#include <Graphics/GpuBuffer.hpp>

namespace Lib::Graphics {
DualBuffer::DualBuffer(const std::shared_ptr<CpuBuffer>& cpuBuffer, const std::shared_ptr<GpuBuffer>& gpuBuffer)
    : m_version()
    , m_cpuBuffer(cpuBuffer)
    , m_gpuBuffer(gpuBuffer)
{
}

void DualBuffer::reset(const std::shared_ptr<CpuBuffer>& cpuBuffer, const std::shared_ptr<GpuBuffer>& gpuBuffer)
{
    m_version = 0;
    m_cpuBuffer = cpuBuffer;
    m_gpuBuffer = gpuBuffer;
}

std::shared_ptr<CpuBuffer> DualBuffer::getCpuBuffer() const { return m_cpuBuffer; }
std::shared_ptr<GpuBuffer> DualBuffer::getGpuBuffer() const { return m_gpuBuffer; }

void DualBuffer::sync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    if (m_version == 0 || m_version != m_cpuBuffer->getVersion()) {
        m_cpuBuffer->transport(cmdList, m_gpuBuffer);
    }
    m_version = m_cpuBuffer->getVersion();
}
}