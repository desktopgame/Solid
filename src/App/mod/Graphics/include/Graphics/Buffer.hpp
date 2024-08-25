#pragma once
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Buffer {
public:
    static std::shared_ptr<Buffer> create();
    ~Buffer();

    void allocate(size_t size);
    void update(const void* data);

    uint64_t getVirtualAddress() const;
    size_t getSize() const;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const;
#endif

private:
    Buffer();

    size_t m_size;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
#endif
};
}