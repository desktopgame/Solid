#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Shader;
class BloomEffect {
public:
#if SOLID_ENABLE_INTERNAL
    static void draw1(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void draw2(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void draw3(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void draw4(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

    static void initialize(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);
    static void destroy();
#endif

private:
    static std::shared_ptr<Shader> s_shader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer;

    static std::shared_ptr<Shader> s_shader2;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState2;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature2;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap2;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer2;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer2;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer2;

    static std::shared_ptr<Shader> s_shader3;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState3;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature3;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap3;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer3;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer3;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer3;

    static std::shared_ptr<Shader> s_shader4;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState4;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature4;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap4;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer4;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer4;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer4;
};
}