#pragma once
#include <Graphics/Device.hpp>
#include <Graphics/PrimitiveType.hpp>
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
class Buffer;
class Constant;
class Shader;
class Swapchain;
class PipelineStateObject;
class TileBatch;
class GlobalLight;
class PointLight;
class Surface {
public:
    ~Surface();

    void guiBegin();
    void guiEnd();

    void begin3D();
    void end3D();

    void begin2D();
    void end2D();

    void render(
        const std::shared_ptr<PipelineStateObject>& pso,
        const std::shared_ptr<Constant>& constant,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

    void render(
        const std::shared_ptr<TileBatch>& tileBatch);

    void effectGlobalLight2D(const Math::Vector3& dir);
    void effectPointLight2D();

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Surface> create(
        const std::shared_ptr<Device>& device,
        const std::shared_ptr<Swapchain>& swapchain);
    void destroy();
#endif

private:
    Surface();

    std::shared_ptr<Swapchain> m_swapchain;
    std::shared_ptr<GlobalLight> m_globalLight;
    std::shared_ptr<PointLight> m_pointLight;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_gHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_gTextures;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_depthStencilViewHeap;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
#endif
};
}