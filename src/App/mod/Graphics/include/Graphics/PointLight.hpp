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
class Device;
class Shader;
class Buffer;
class PointLight {
public:
    static inline constexpr int32_t k_maxCount = 50;

#if SOLID_ENABLE_INTERNAL
    void clear();
    void draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const Math::Vector3& position, float innerRadius, float outerRadius);

    static std::shared_ptr<PointLight> create(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures);
    void destroy();
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
        std::array<float, 59> padding;
    };
    static_assert(sizeof(Constant2) == 256);

    PointLight();
    static void generateSphere(int32_t radius, int32_t latitudes, int32_t longitudes, std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices);

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Shader> m_scrShader;
    int32_t m_vertexLength;
    int32_t m_indexLength;
    int32_t m_currentLightIndex;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_scrPipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_scrRootSignature;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_scrDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_scrVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_scrIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_scrConstantBuffer;
#endif
};
}