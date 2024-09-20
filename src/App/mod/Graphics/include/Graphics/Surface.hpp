#pragma once
#include <Graphics/Device.hpp>
#include <Graphics/PrimitiveType.hpp>
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
class Surface {
public:
    ~Surface();

    void guiBegin();
    void guiEnd();

    void begin();
    void end();

    void render(
        const std::shared_ptr<PipelineStateObject>& pso,
        const std::shared_ptr<Constant>& constant,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

    void render(
        const std::shared_ptr<TileBatch>& tileBatch);

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Surface> create(
        const std::shared_ptr<Device>& device,
        const std::shared_ptr<Swapchain>& swapchain);
    void destroy();
#endif

private:
    Surface();

    std::shared_ptr<Swapchain> m_swapchain;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
#endif
};
}