#pragma once
#include <Graphics/Metadata.hpp>
#include <d3d12.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

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

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getID3D12DescriptorHeap() const;
#endif

private:
    UniformBuffer();
    void init(Metadata::ProgramTable entry);

    Metadata::ProgramTable m_entry;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_vsResources;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_psResources;
};
}