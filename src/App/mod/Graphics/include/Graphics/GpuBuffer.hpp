#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class GpuBuffer {
public:
    enum class Type {
        Vertex,
        ReadWrite
    };

    static std::shared_ptr<GpuBuffer> create(Type type);
    ~GpuBuffer();

    void allocate(size_t size);
    void update(const void* data);

    Type getType() const;
    size_t getSize() const;
    int32_t getVersion() const;

#if SOLID_ENABLE_INTERNAL
    void stateUAV(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    void stateCommon(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    void stateSync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

    void transport(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<GpuBuffer>& dst);
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const;
#endif

private:
    GpuBuffer();

    Type m_type;
    size_t m_size;
    int32_t m_version;

    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
};
}