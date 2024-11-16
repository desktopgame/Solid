#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Shader;
class BloomEffect {
public:
#if SOLID_ENABLE_INTERNAL
    static void draw1(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void draw2(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

    static void initialize(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);
    static void destroy();
#endif

private:
    static std::shared_ptr<Shader> s_shader;
    static std::shared_ptr<Shader> s_shader2;

#if SOLID_ENABLE_INTERNAL
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState2;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature2;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap2;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer2;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer2;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer2;
#endif
};
}