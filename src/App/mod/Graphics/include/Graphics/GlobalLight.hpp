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

    static void enable();
    static void disable();
    static void set(const Math::Vector3& dir);

#if SOLID_ENABLE_INTERNAL
    static void draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

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

    static std::shared_ptr<Shader> s_shader;
    static bool s_enabled;
    static Math::Vector3 s_dir;
    static Constant s_constantData;

#if SOLID_ENABLE_INTERNAL
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> s_pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> s_rootSignature;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> s_descriptorHeap;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_vertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_indexBuffer;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_constantBuffer;
#endif
};
}