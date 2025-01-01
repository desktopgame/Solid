#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class CpuBuffer;
class GpuBuffer;

class DualBuffer {
public:
    explicit DualBuffer(const std::shared_ptr<CpuBuffer>& cpuBuffer, const std::shared_ptr<GpuBuffer>& gpuBuffer);

    void reset(const std::shared_ptr<CpuBuffer>& cpuBuffer, const std::shared_ptr<GpuBuffer>& gpuBuffer);

    std::shared_ptr<CpuBuffer> getCpuBuffer() const;
    std::shared_ptr<GpuBuffer> getGpuBuffer() const;

#if SOLID_ENABLE_INTERNAL
    void sync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
#endif

private:
    int32_t m_version;
    std::shared_ptr<CpuBuffer> m_cpuBuffer;
    std::shared_ptr<GpuBuffer> m_gpuBuffer;
};
}