#pragma once
#include <Graphics/Metadata.hpp>
#include <d3d12.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Texture;
class Buffer;
class UniformBuffer {
public:
    void setVS(int32_t index, const void* data);
    void setGS(int32_t index, const void* data);
    void setPS(int32_t index, const void* data);
    void setPS(int32_t index, const std::shared_ptr<Texture>& texture);
    void setCS(int32_t index, const void* data);
    void setCS(int32_t index, const std::shared_ptr<Texture>& texture);
    void setCS(int32_t index, const std::shared_ptr<Buffer>& buffer);

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<UniformBuffer> create(Metadata::ProgramTable entry);
    void destroy();

    void stateUAV(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    void stateCommon(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    void stateSync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getID3D12DescriptorHeap() const;
#endif

private:
    UniformBuffer();
    void init(Metadata::ProgramTable entry);

    Metadata::ProgramTable m_entry;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_vsResources;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_gsResources;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_psResources;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_csResources;
    std::vector<std::shared_ptr<Buffer>> m_uavBuffers;
};
}