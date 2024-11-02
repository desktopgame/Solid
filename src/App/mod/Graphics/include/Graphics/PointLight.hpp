#pragma once
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Device;
class Shader;
class Buffer;
class PointLight {
public:
#if SOLID_ENABLE_INTERNAL
    void draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

    static std::shared_ptr<PointLight> create(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures);
    void destroy();
#endif

private:
    PointLight();

    std::shared_ptr<Shader> m_shader;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
#endif
};
}