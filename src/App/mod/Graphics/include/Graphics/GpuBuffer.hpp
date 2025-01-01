#pragma once
#include <Graphics/IBuffer.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class GpuBuffer : public IBuffer {
public:
    static std::shared_ptr<GpuBuffer> create();
    ~GpuBuffer();

    void allocate(size_t size);
    size_t getSize() const override;

#if SOLID_ENABLE_INTERNAL
    void beginCompute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    void endCompute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    void syncCompute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const override;
#endif

private:
    GpuBuffer();

    size_t m_size;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
};
}