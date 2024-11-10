#pragma once
#include <Graphics/Metadata.hpp>
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>

class ID3D12GraphicsCommandList;
#endif

namespace Lib::Graphics {
class Texture;
class UniformBuffer {
public:
    void setVS(int32_t index, const void* data);
    void setPS(int32_t index, const void* data);
    void setPS(int32_t index, const std::shared_ptr<Texture>& texture);

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<UniformBuffer> create(Metadata::ProgramTable entry);
    void destroy();
#endif

private:
    UniformBuffer();

    Metadata::ProgramTable m_entry;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_vsResources;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_psResources;
#endif
};
}