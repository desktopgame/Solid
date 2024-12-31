#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class Buffer;

class DualBuffer {
public:
    explicit DualBuffer(const std::shared_ptr<Buffer>& cpuBuffer, const std::shared_ptr<Buffer>& gpuBuffer);

    std::shared_ptr<Buffer> getCpuBuffer() const;
    std::shared_ptr<Buffer> getGpuBuffer() const;

#if SOLID_ENABLE_INTERNAL
    void sync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
#endif

private:
    int32_t m_version;
    std::shared_ptr<Buffer> m_cpuBuffer;
    std::shared_ptr<Buffer> m_gpuBuffer;
};
}