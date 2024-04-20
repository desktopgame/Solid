#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Internal/Swapchain.hpp>
#include <Graphics/Window.hpp>
#include <Windows.h>
#include <any>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdexcept>
#include <vector>

namespace Lib::Graphics::Internal {
using Microsoft::WRL::ComPtr;
// Impl
class Swapchain::Impl {
public:
    explicit Impl() = default;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<IDXGISwapChain4> swapchain;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps;
    std::vector<ComPtr<ID3D12Resource>> renderTargetViews;
    ComPtr<ID3D12Fence> fence;
};
// public
Swapchain::~Swapchain()
{
}

void Swapchain::target(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    uint32_t backBufferIndex
        = m_impl->swapchain->GetCurrentBackBufferIndex();
    std::any deviceHandle = Engine::getInstance()->getDevice()->getHandle();
    auto nativeDevice = std::any_cast<ComPtr<ID3D12Device>>(deviceHandle);
    // Barrier
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_impl->renderTargetViews.at(backBufferIndex).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_impl->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * nativeDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    commandList->OMSetRenderTargets(1, &rtvHandle, true, nullptr);

    float clearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Swapchain::execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    ID3D12CommandList* cmdLists[] = { commandList.Get() };
    m_impl->commandQueue->ExecuteCommandLists(1, cmdLists);
}

void Swapchain::present(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    uint32_t backBufferIndex
        = m_impl->swapchain->GetCurrentBackBufferIndex();
    // Barrier
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_impl->renderTargetViews.at(backBufferIndex).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    commandList->ResourceBarrier(1, &barrier);
    m_impl->swapchain->Present(1, 0);
}

void Swapchain::waitSync()
{
    // Wait events
    m_impl->commandQueue->Signal(m_impl->fence.Get(), ++m_fenceVal);
    if (m_impl->fence->GetCompletedValue() != m_fenceVal) {
        HANDLE evt = CreateEvent(nullptr, false, false, nullptr);
        m_impl->fence->SetEventOnCompletion(m_fenceVal, evt);
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }
}
// private
Swapchain::Swapchain()
    : m_impl(std::make_shared<Impl>())
{
}

std::shared_ptr<Swapchain> Swapchain::create(
    const std::shared_ptr<Window>& window,
    const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory,
    const Microsoft::WRL::ComPtr<ID3D12Device>& device)
{
    HWND hwnd = std::any_cast<HWND>(window->getHandle());
    auto nativeDevice = device;
    auto swapchain = std::shared_ptr<Swapchain>(new Swapchain());
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
    swapchain->m_impl->commandQueue = commandQueue;
    // Swapchain
    ComPtr<IDXGISwapChain4> nativeSwapchain = nullptr;
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
    if (FAILED(dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)nativeSwapchain.ReleaseAndGetAddressOf()))) {
        throw std::runtime_error("failed CreateSwapChainForHwnd()");
    }
    swapchain->m_impl->swapchain = nativeSwapchain;
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
    swapchain->m_impl->rtvHeaps = rtvHeaps;
    // RenderTargetView
    std::vector<ComPtr<ID3D12Resource>> renderTargetViews(2);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    for (uint32_t i = 0; i < swapchainDesc.BufferCount; i++) {
        if (FAILED(nativeSwapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargetViews.at(i))))) {
            throw std::runtime_error("failed GetBuffer()");
        }
        nativeDevice->CreateRenderTargetView(renderTargetViews.at(i).Get(), nullptr, handle);
        handle.ptr += nativeDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    swapchain->m_impl->renderTargetViews = renderTargetViews;
    // Fence
    ComPtr<ID3D12Fence> fence = nullptr;
    if (FAILED(nativeDevice->CreateFence(swapchain->m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
        throw std::runtime_error("failed CreateFence()");
    }
    swapchain->m_impl->fence = fence;
    return swapchain;
}
}