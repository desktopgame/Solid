#include <Graphics/Buffer.hpp>
#include <Graphics/DualBuffer.hpp>

namespace Lib::Graphics {
DualBuffer::DualBuffer(const std::shared_ptr<Buffer>& cpuBuffer, const std::shared_ptr<Buffer>& gpuBuffer)
    : m_version()
    , m_cpuBuffer(cpuBuffer)
    , m_gpuBuffer(gpuBuffer)
{
}

std::shared_ptr<Buffer> DualBuffer::getCpuBuffer() const { return m_cpuBuffer; }
std::shared_ptr<Buffer> DualBuffer::getGpuBuffer() const { return m_gpuBuffer; }

void DualBuffer::sync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    if (m_version == 0 || m_version != m_cpuBuffer->getVersion()) {
        m_cpuBuffer->transport(cmdList, m_gpuBuffer);
    }
    m_version = m_cpuBuffer->getVersion();
}
}