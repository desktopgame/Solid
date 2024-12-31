#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class Buffer {
public:
    enum class Type {
        Vertex,
        ReadWrite
    };

    static std::shared_ptr<Buffer> create(Type type);
    ~Buffer();

    void allocate(size_t size);
    void update(const void* data);

    size_t getSize() const;

#if SOLID_ENABLE_INTERNAL
    void transport(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<Buffer>& dst);
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const;
#endif

private:
    Buffer();

    Type m_type;
    size_t m_size;

    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
};
}