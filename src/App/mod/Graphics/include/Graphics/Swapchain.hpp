#pragma once
#include <Graphics/Device.hpp>
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Window;
class Device;
class Swapchain {
public:
    ~Swapchain();

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Swapchain> create(
        const std::shared_ptr<Window>& window,
        const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory,
        const Microsoft::WRL::ComPtr<ID3D12Device>& device);

    void guiClear();
    void guiRender();

    void clear(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void swap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void present(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void waitSync();

    void destroy();
#endif

private:
    Swapchain();

    uint64_t m_fenceVal;
    bool m_renderGui;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapchain;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeaps;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_renderTargetViews;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_depthStencilViewHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_imguiDescriptorHeap;
#endif
};
}