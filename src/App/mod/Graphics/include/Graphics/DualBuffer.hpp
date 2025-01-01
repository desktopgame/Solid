#pragma once
#include <Graphics/IBuffer.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class CpuBuffer;
class GpuBuffer;

class DualBuffer : public IBuffer {
public:
    static std::shared_ptr<DualBuffer> create();

    void allocate(size_t size);
    void update(const void* data);
    void reset(const std::shared_ptr<CpuBuffer>& cpuBuffer, const std::shared_ptr<GpuBuffer>& gpuBuffer);

    std::shared_ptr<CpuBuffer> getCpuBuffer() const;
    std::shared_ptr<GpuBuffer> getGpuBuffer() const;
    size_t getSize() const override;

#if SOLID_ENABLE_INTERNAL
    void sync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const override;
#endif

private:
    DualBuffer();
    int32_t m_version;
    std::shared_ptr<CpuBuffer> m_cpuBuffer;
    std::shared_ptr<GpuBuffer> m_gpuBuffer;
};
}