#pragma once
#include <Graphics/Device.hpp>
#include <Math/Vector.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Device;
class Buffer;
class Constant;
class Shader;
class Swapchain;
class TileBatch;
class RenderContext;
class UniformBuffer;
class Surface {
public:
    ~Surface();

    void beginGui();
    void endGui();

    void begin3D();
    void end3D();

    void begin2D();
    void end2D();

    void present();

    void compute(const std::shared_ptr<RenderContext>& rc,
        const std::shared_ptr<UniformBuffer>& ub,
        int32_t threadGroupCountX,
        int32_t threadGroupCountY,
        int32_t threadGroupCountZ);

    void render(
        const std::shared_ptr<RenderContext>& rc,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<Buffer>& vertex,
        const std::shared_ptr<Buffer>& index,
        int32_t indexLength);

    void render(
        const std::shared_ptr<RenderContext>& rc,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<Buffer>& vertex,
        const std::shared_ptr<Buffer>& index,
        int32_t indexLength,
        const std::vector<std::shared_ptr<Buffer>>& instanceBuffers,
        int32_t instanceCount);

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Surface> create(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::shared_ptr<Swapchain>& swapchain);
    void destroy();
#endif

private:
    Surface();
    void init(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::shared_ptr<Swapchain>& swapchain);

    void bloomWrite(int32_t index);
    void bloomRead(int32_t index);

    std::shared_ptr<Swapchain> m_swapchain;

    Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_gHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_gTextures;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_bloomHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_bloomTextures;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_depthStencilViewHeap;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
};
}