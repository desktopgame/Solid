#pragma once
#include <Graphics/IBuffer.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class GpuBuffer;
class CpuBuffer : public IBuffer {
public:
    static std::shared_ptr<CpuBuffer> create();
    ~CpuBuffer();

    void allocate(size_t size);
    void update(const void* data);

    size_t getSize() const override;
    int32_t getVersion() const;

#if SOLID_ENABLE_INTERNAL
    void transport(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<GpuBuffer>& dst);
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const override;
#endif

private:
    CpuBuffer();

    size_t m_size;
    int32_t m_version;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
};
}