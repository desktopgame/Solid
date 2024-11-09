#pragma once
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
class GlobalLight {
public:
    static inline constexpr int32_t k_maxCount = 1;

#if SOLID_ENABLE_INTERNAL
    static void clear();
    static void draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, const Math::Vector3& dir);

    static void initialize(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures);
    static void destroy();
#endif

private:
    struct Constant {
    public:
        Math::Vector3 direction;
        std::array<float, 61> padding;
    };
    static_assert(sizeof(Constant) == 256);

    GlobalLight() = delete;

    static std::shared_ptr<Shader> m_shader;
    static bool m_drawLight;
    static Constant m_constantData;

#if SOLID_ENABLE_INTERNAL
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
#endif
};
}