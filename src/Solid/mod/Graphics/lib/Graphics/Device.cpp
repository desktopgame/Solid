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
// Impl
class Device::Impl {
public:
    explicit Impl() = default;
    HWND hwnd;
    ComPtr<IDXGIFactory6> dxgiFactory;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12InfoQueue> infoQueue;
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> commandList;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<IDXGISwapChain4> swapchain;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps;
    std::vector<ComPtr<ID3D12Resource>> renderTargetViews;
    ComPtr<ID3D12Fence> fence;

private:
};
// public
Device::~Device()
{
}

void Device::flushLogEntries()
{
    // Show messages
    ComPtr<ID3D12InfoQueue> infoQueue = m_impl->infoQueue;
    uint64_t messageCount = infoQueue->GetNumStoredMessages();
    for (uint64_t i = 0; i < messageCount; i++) {
        size_t messageSize = 0;
        if (SUCCEEDED(infoQueue->GetMessage(i, nullptr, &messageSize))) {
            D3D12_MESSAGE* message = (D3D12_MESSAGE*)::malloc(messageSize);
            if (SUCCEEDED(infoQueue->GetMessage(i, message, &messageSize))) {
                bool isInfo = message->Severity != D3D12_MESSAGE_SEVERITY_INFO && message->Severity != D3D12_MESSAGE_SEVERITY_MESSAGE;
                if (!isInfo) {
                    std::cout << message->pDescription << std::endl;
                }
            }
            ::free(message);
        }
    }
    infoQueue->ClearStoredMessages();
}

void Device::render()
{
    // Main loop
    m_impl->commandAllocator->Reset();
    uint32_t backBufferIndex
        = m_impl->swapchain->GetCurrentBackBufferIndex();
    // Barrier
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_impl->renderTargetViews.at(backBufferIndex).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_impl->commandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_impl->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * m_impl->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_impl->commandList->OMSetRenderTargets(1, &rtvHandle, true, nullptr);

    float clearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    m_impl->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    m_impl->commandList->Close();

    ID3D12CommandList* cmdLists[] = { m_impl->commandList.Get() };
    m_impl->commandQueue->ExecuteCommandLists(1, cmdLists);

    m_impl->commandAllocator->Reset();
    m_impl->commandList->Reset(m_impl->commandAllocator.Get(), nullptr);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_impl->commandList->ResourceBarrier(1, &barrier);
    m_impl->swapchain->Present(1, 0);
}
void Device::notify()
{
    m_impl->commandQueue->Signal(m_impl->fence.Get(), ++m_fenceVal);
}

void Device::waitEvents()
{
    if (m_impl->fence->GetCompletedValue() != m_fenceVal) {
        HANDLE evt = CreateEvent(nullptr, false, false, nullptr);
        m_impl->fence->SetEventOnCompletion(m_fenceVal, evt);
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }
}
// private
Device::Device()
    : m_fenceVal()
    , m_impl(std::make_shared<Impl>())
{
}

std::shared_ptr<Device> Device::create(const std::shared_ptr<Window>& window)
{
    std::any hwnd = window->getHandle();
    auto device = std::shared_ptr<Device>(new Device());
    device->m_impl->hwnd = std::any_cast<HWND>(hwnd);
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
    device->m_impl->dxgiFactory = dxgiFactory;
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
    device->m_impl->device = nativeDevice;
    // InfoQueue
    ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
    if (FAILED(nativeDevice->QueryInterface(__uuidof(ID3D12InfoQueue), &infoQueue))) {
        throw std::runtime_error("failed ID3D12InfoQueue()");
    }
    device->m_impl->infoQueue = infoQueue;
    // CommandAllocator
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    if (FAILED(nativeDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) {
        throw std::runtime_error("failed CreateCommandAllocator()");
    }
    device->m_impl->commandAllocator = commandAllocator;
    // CommandList
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    if (FAILED(nativeDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) {
        throw std::runtime_error("failed CreateCommandList()");
    }
    device->m_impl->commandList = commandList;
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
    device->m_impl->commandQueue = commandQueue;
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
    device->m_impl->swapchain = swapchain;
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
    device->m_impl->rtvHeaps = rtvHeaps;
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
    device->m_impl->renderTargetViews = renderTargetViews;
    // Fence
    ComPtr<ID3D12Fence> fence = nullptr;
    if (FAILED(nativeDevice->CreateFence(device->m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
        throw std::runtime_error("failed CreateFence()");
    }
    device->m_impl->fence = fence;
    return device;
}

void Device::destroy()
{
    m_impl = nullptr;
}
}