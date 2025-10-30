#pragma once
#include <Graphics/Device.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::OS {
class Window;
}

namespace Lib::Graphics {
class Device;
/**
 * サーフェイスよりさらに低レベルな描画処理を扱います。
 * ユーザーはこのクラスを使う必要がありません。
 */
class Swapchain {
public:
    ~Swapchain();

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Swapchain> create(
        const std::shared_ptr<OS::Window>& window,
        const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory,
        const Microsoft::WRL::ComPtr<ID3D12Device>& device);

    void guiClear();
    void guiRender();

    void clear(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void swap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void present();
    void fence();
    void signal();
    void waitSync();

    void destroy();
#endif

private:
    Swapchain();
    void init(
        const std::shared_ptr<OS::Window>& window,
        const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory,
        const Microsoft::WRL::ComPtr<ID3D12Device>& device);

    uint64_t m_fenceVal;
    bool m_renderGui;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapchain;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_renderTargetViewHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_renderTargetViews;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_imguiDescriptorHeap;
};
}