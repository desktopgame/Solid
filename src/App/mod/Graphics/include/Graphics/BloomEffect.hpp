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
    static void drawFilter(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void drawBlur1(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void drawBlur2(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    static void drawMix(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

    static void initialize(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);
    static void destroy();
#endif

private:
    BloomEffect() = delete;
    static void initFilter(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);
    static void initBlur1(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);
    static void initBlur2(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);
    static void initMix(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures);

    static std::shared_ptr<Shader> s_filterShader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_filterPipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_filterRootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_filterDescriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_filterVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_filterIndexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_filterConstantBuffer;

    static std::shared_ptr<Shader> s_blur1Shader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_blur1PipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_blur1RootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_blur1DescriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_blur1VertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_blur1IndexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_blur1ConstantBuffer;

    static std::shared_ptr<Shader> s_blur2Shader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_blur2PipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_blur2RootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_blur2DescriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_blur2VertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_blur2IndexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_blur2ConstantBuffer;

    static std::shared_ptr<Shader> s_mixShader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_mixPipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_mixRootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_mixDescriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_mixVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_mixIndexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_mixConstantBuffer;
};
}