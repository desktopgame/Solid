#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Device;
class Shader;
class CpuBuffer;
class PointLight {
public:
    static inline constexpr int32_t k_maxCount = 1000;

    static void enable();
    static void disable();
    static void toggle(bool enabled);
    static void set(int32_t index, const Math::Vector3& position, float innerRadius, float outerRadius, const Math::Vector3& color);
    static void setCount(int32_t count);

#if SOLID_ENABLE_INTERNAL
    static void draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

    static void initialize(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures);
    static void destroy();
#endif

private:
    struct Constant1 {
    public:
        Math::Matrix modelMatrix;
        Math::Matrix viewMatrix;
        Math::Matrix projectionMatrix;
        std::array<float, 16> padding;
    };
    static_assert(sizeof(Constant1) == 256);

    struct Constant2 {
    public:
        Math::Vector3 position;
        float innerRadius;
        float outerRadius;
        Math::Vector3 color;
        std::array<float, 56> padding;
    };
    static_assert(sizeof(Constant2) == 256);

    PointLight() = delete;
    static void initStencil(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures);
    static void initLight(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures);

    static void drawStencil(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, int32_t lightIndex);
    static void drawLight(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, int32_t lightIndex);
    static void generateSphere(int32_t radius, int32_t latitudes, int32_t longitudes, std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices);

    static int32_t s_vertexLength;
    static int32_t s_indexLength;
    static bool s_enabled;
    static std::vector<Constant2> s_constantVec;
    static int32_t s_count;

    static std::shared_ptr<Shader> s_vShader;
    static std::shared_ptr<Shader> s_pShader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer;

    static std::shared_ptr<Shader> s_scrVShader;
    static std::shared_ptr<Shader> s_scrPShader;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_scrPipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_scrRootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_scrDescriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_scrVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_scrIndexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_scrConstantBuffer;
};
}