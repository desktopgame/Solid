#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Swapchain.hpp>
#include <OS/Window.hpp>
#include <any>
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <stdexcept>
#include <vector>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
Swapchain::~Swapchain()
{
}
// internal
std::shared_ptr<Swapchain> Swapchain::create(
    const std::shared_ptr<OS::Window>& window,
    const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory,
    const Microsoft::WRL::ComPtr<ID3D12Device>& device)
{
    HWND hwnd = window->getHWND();
    auto swapchain = std::shared_ptr<Swapchain>(new Swapchain());
    // CommandQueue
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandQueueDesc.NodeMask = 0;
    commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&swapchain->m_commandQueue)))) {
        throw std::runtime_error("failed CreateCommandQueue()");
    }
    // Swapchain
    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    swapchainDesc.Width = Screen::getWidth();
    swapchainDesc.Height = Screen::getHeight();
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
    if (FAILED(dxgiFactory->CreateSwapChainForHwnd(swapchain->m_commandQueue.Get(), hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)swapchain->m_swapchain.ReleaseAndGetAddressOf()))) {
        throw std::runtime_error("failed CreateSwapChainForHwnd()");
    }
    // DescriptorHeap
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NodeMask = 0;
    heapDesc.NumDescriptors = 2;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&swapchain->m_renderTargetViewHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    // RenderTargetView
    std::vector<ComPtr<ID3D12Resource>> renderTargetViews(2);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = swapchain->m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
    for (uint32_t i = 0; i < swapchainDesc.BufferCount; i++) {
        if (FAILED(swapchain->m_swapchain->GetBuffer(i, IID_PPV_ARGS(&renderTargetViews.at(i))))) {
            throw std::runtime_error("failed GetBuffer()");
        }
        device->CreateRenderTargetView(renderTargetViews.at(i).Get(), nullptr, handle);
        handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    swapchain->m_renderTargetViews = renderTargetViews;
    // DepthBuffer
    D3D12_RESOURCE_DESC depthResDesc = {};
    depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResDesc.Width = Screen::getWidth();
    depthResDesc.Height = Screen::getHeight();
    depthResDesc.DepthOrArraySize = 1;
    depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthResDesc.SampleDesc.Count = 1;
    depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    D3D12_HEAP_PROPERTIES depthHeapProps = {};
    depthHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    depthHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    depthHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    D3D12_CLEAR_VALUE depthClearValue = {};
    depthClearValue.DepthStencil.Depth = 1.0f;
    depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    if (FAILED(device->CreateCommittedResource(&depthHeapProps, D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&swapchain->m_depthBuffer)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    swapchain->m_depthBuffer->SetName(L"DepthBuffer");
    // DepthStencilViewHeap
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    if (FAILED(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&swapchain->m_depthStencilViewHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    // DepthBufferView
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    device->CreateDepthStencilView(swapchain->m_depthBuffer.Get(), &dsvDesc, swapchain->m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
    // Fence
    if (FAILED(device->CreateFence(swapchain->m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&swapchain->m_fence)))) {
        throw std::runtime_error("failed CreateFence()");
    }
    // ImGui
    D3D12_DESCRIPTOR_HEAP_DESC imguiDescriptorHeapDesc = {};
    imguiDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    imguiDescriptorHeapDesc.NumDescriptors = 1;
    imguiDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (FAILED(device->CreateDescriptorHeap(&imguiDescriptorHeapDesc, IID_PPV_ARGS(&swapchain->m_imguiDescriptorHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(device.Get(), 3,
        DXGI_FORMAT_R8G8B8A8_UNORM, swapchain->m_imguiDescriptorHeap.Get(),
        swapchain->m_imguiDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        swapchain->m_imguiDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    return swapchain;
}

void Swapchain::guiClear()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Swapchain::guiRender()
{
    m_renderGui = true;
    ImGui::Render();
}

void Swapchain::clear(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    uint32_t backBufferIndex = m_swapchain->GetCurrentBackBufferIndex();
    auto d3d12Device = Engine::getInstance()->getDevice()->getID3D12Device();
    // Barrier
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_renderTargetViews.at(backBufferIndex).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
    commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

    float clearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // viewport
    D3D12_VIEWPORT viewport = {};
    viewport.Width = Screen::getWidth();
    viewport.Height = Screen::getHeight();
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    commandList->RSSetViewports(1, &viewport);
    // scissor
    D3D12_RECT scissorRect = {};
    scissorRect.top = 0;
    scissorRect.left = 0;
    scissorRect.right = scissorRect.left + Screen::getWidth();
    scissorRect.bottom = scissorRect.top + Screen::getHeight();
    commandList->RSSetScissorRects(1, &scissorRect);
}

void Swapchain::execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    ID3D12CommandList* cmdLists[] = { commandList.Get() };
    m_commandQueue->ExecuteCommandLists(1, cmdLists);
}

void Swapchain::swap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    if (m_renderGui) {
        commandList->SetDescriptorHeaps(1, m_imguiDescriptorHeap.GetAddressOf());
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
        m_renderGui = false;
    }

    uint32_t backBufferIndex = m_swapchain->GetCurrentBackBufferIndex();
    // Barrier
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_renderTargetViews.at(backBufferIndex).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    commandList->ResourceBarrier(1, &barrier);
}

void Swapchain::present(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    m_swapchain->Present(1, 0);
}

void Swapchain::waitSync()
{
    // Wait events
    m_commandQueue->Signal(m_fence.Get(), ++m_fenceVal);
    if (m_fence->GetCompletedValue() != m_fenceVal) {
        HANDLE evt = CreateEvent(nullptr, false, false, nullptr);
        m_fence->SetEventOnCompletion(m_fenceVal, evt);
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }
}

void Swapchain::destroy()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
}
// private
Swapchain::Swapchain()
    : m_fenceVal(0)
    , m_renderGui()
    , m_commandQueue()
    , m_swapchain()
    , m_renderTargetViewHeap()
    , m_renderTargetViews()
    , m_fence()
    , m_depthBuffer()
    , m_depthStencilViewHeap()
    , m_imguiDescriptorHeap()
{
}
}