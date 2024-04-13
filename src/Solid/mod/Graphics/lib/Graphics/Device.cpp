#include <Graphics/Device.hpp>
#include <Graphics/Window.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
Device::~Device()
{
}
// Impl
class Device::Impl {
public:
    explicit Impl() = default;
    HWND hwnd;
    ComPtr<ID3D12Device> device;

private:
};
// private
Device::Device()
    : m_impl(std::make_shared<Impl>())
{
}

std::shared_ptr<Device> Device::create(const std::shared_ptr<Window>& window)
{
    std::any hwnd = window->getHandle();
    auto device = std::shared_ptr<Device>(new Device());
    // Debug Layer
    ComPtr<ID3D12Debug> debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
    // Factory
    ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
    UINT flags = DXGI_CREATE_FACTORY_DEBUG;
    if (FAILED(CreateDXGIFactory2(flags, IID_PPV_ARGS(&dxgiFactory)))) {
        throw std::runtime_error("failed CreateDXGIFactory2()");
    }
    // Adapter
    std::vector<ComPtr<IDXGIAdapter>> adapters;
    ComPtr<IDXGIAdapter> mainAdapter = nullptr;
    for (uint32_t i = 0; dxgiFactory->EnumAdapters(i, &mainAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
        adapters.emplace_back(mainAdapter);
    }
    if (adapters.empty()) {
        throw std::runtime_error("failed EnumAdapters()");
    }
    // first adapter is primary adapter.
    // see: https://learn.microsoft.com/ja-jp/windows/win32/api/dxgi/nf-dxgi-idxgifactory-enumadapters
    mainAdapter = adapters.at(0);
    adapters.clear();
    // Feature level
    std::optional<D3D_FEATURE_LEVEL> useLevel;
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
    };
    ComPtr<ID3D12Device> nativeDevice = nullptr;
    for (D3D_FEATURE_LEVEL level : levels) {
        HRESULT deviceStatus = D3D12CreateDevice(mainAdapter.Get(), level, IID_PPV_ARGS(&nativeDevice));
        if (SUCCEEDED(deviceStatus)) {
            useLevel = level;
            break;
        }
    }
    if (!useLevel) {
        throw std::runtime_error("failed D3D12CreateDevice()");
    }
    device->m_impl->hwnd = std::any_cast<HWND>(device);
    device->m_impl->device = nativeDevice;
    // InfoQueue
    ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
    if (FAILED(nativeDevice->QueryInterface(__uuidof(ID3D12InfoQueue), &infoQueue))) {
        throw std::runtime_error("failed ID3D12InfoQueue()");
    }
    // CommandAllocator
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    if (FAILED(nativeDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) {
        throw std::runtime_error("failed CreateCommandAllocator()");
    }
    // CommandList
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    if (FAILED(nativeDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) {
        throw std::runtime_error("failed CreateCommandList()");
    }
    // CommandQueue
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandQueueDesc.NodeMask = 0;
    commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(nativeDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)))) {
        throw std::runtime_error("failed CreateCommandQueue()");
    }
    // Swapchain
    ComPtr<IDXGISwapChain4> swapchain = nullptr;
    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    swapchainDesc.Width = 800;
    swapchainDesc.Height = 600;
    swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.Stereo = false;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    if (FAILED(dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), device->m_impl->hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)swapchain.ReleaseAndGetAddressOf()))) {
        throw std::runtime_error("failed CreateSwapChainForHwnd()");
    }
    // DescriptorHeap
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NodeMask = 0;
    heapDesc.NumDescriptors = 2;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
    if (FAILED(nativeDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    // RenderTargetView
    std::vector<ComPtr<ID3D12Resource>> renderTargetViews(2);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    for (uint32_t i = 0; i < swapchainDesc.BufferCount; i++) {
        if (FAILED(swapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargetViews.at(i))))) {
            throw std::runtime_error("failed GetBuffer()");
        }
        nativeDevice->CreateRenderTargetView(renderTargetViews.at(i).Get(), nullptr, handle);
        handle.ptr += nativeDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    // Fence
    ComPtr<ID3D12Fence> fence = nullptr;
    uint64_t fenceVal = 0;
    if (FAILED(nativeDevice->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
        throw std::runtime_error("failed CreateFence()");
    }

    return device;
}
}