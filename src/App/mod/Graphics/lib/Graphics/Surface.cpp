#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/GlobalLight.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/PointLight.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Swapchain.hpp>
#include <Graphics/TileBatch.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Graphics/VertexTexCoord3D.hpp>
#include <Math/Vector.hpp>
#include <stdexcept>
#include <vector>

namespace Lib::Graphics {

using Microsoft::WRL::ComPtr;
// public
Surface::~Surface()
{
}

void Surface::guiBegin()
{
    m_swapchain->guiClear();
}

void Surface::guiEnd()
{
    m_swapchain->guiRender();
}

void Surface::begin3D()
{
    auto d3d12Device = Engine::getInstance()->getDevice()->getID3D12Device();
    // Barrier
    D3D12_RESOURCE_BARRIER barriers[3] = {};
    for (int32_t i = 0; i < 3; i++) {
        D3D12_RESOURCE_BARRIER& barrier = barriers[i];
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_gTextures.at(i).Get();
        barrier.Transition.Subresource = 0;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    m_commandList->ResourceBarrier(3, barriers);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_gHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
    m_commandList->OMSetRenderTargets(3, &rtvHandle, true, &dsvHandle);

    float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (int32_t i = 0; i < 3; i++) {
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        rtvHandle.ptr += d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    D3D12_CLEAR_FLAGS clearDepthFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
    m_commandList->ClearDepthStencilView(dsvHandle, clearDepthFlags, 1.0f, 0, 0, nullptr);
    m_commandList->OMSetStencilRef(1);

    // viewport
    D3D12_VIEWPORT viewports[3] = {};
    for (int32_t i = 0; i < 3; i++) {
        D3D12_VIEWPORT& viewport = viewports[i];
        viewport.Width = Screen::getWidth();
        viewport.Height = Screen::getHeight();
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MaxDepth = 1.0f;
        viewport.MinDepth = 0.0f;
    }
    m_commandList->RSSetViewports(3, viewports);
    // scissor
    D3D12_RECT scissorRects[3] = {};
    for (int32_t i = 0; i < 3; i++) {
        D3D12_RECT& scissorRect = scissorRects[i];
        scissorRect.top = 0;
        scissorRect.left = 0;
        scissorRect.right = scissorRect.left + Screen::getWidth();
        scissorRect.bottom = scissorRect.top + Screen::getHeight();
    }
    m_commandList->RSSetScissorRects(3, scissorRects);
}

void Surface::end3D()
{
    // Barrier
    D3D12_RESOURCE_BARRIER barriers[3] = {};
    for (int32_t i = 0; i < 3; i++) {
        D3D12_RESOURCE_BARRIER& barrier = barriers[i];
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_gTextures.at(i).Get();
        barrier.Transition.Subresource = 0;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    }
    m_commandList->ResourceBarrier(3, barriers);

    m_commandList->OMSetStencilRef(2);
}

void Surface::begin2D()
{
    m_swapchain->clear(m_commandList, m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());

    // light
    GlobalLight::draw(m_commandList);
    PointLight::draw(m_commandList);
}

void Surface::end2D()
{
    m_swapchain->swap(m_commandList);
    m_commandList->Close();
    m_swapchain->execute(m_commandList);

    m_swapchain->present(m_commandList);
    m_swapchain->waitSync();

    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator.Get(), nullptr);
}

void Surface::render(
    const std::shared_ptr<PipelineStateObject>& pso,
    const std::shared_ptr<Constant>& constant,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength)
{
    constant->update();
    pso->render(m_commandList, constant, vertexBuffer, indexBuffer, indexLength);
}

void Surface::render(const std::shared_ptr<TileBatch>& tileBatch)
{
    tileBatch->render(m_commandList);
}
// internal
std::shared_ptr<Surface> Surface::create(
    const std::shared_ptr<Device>& device,
    const std::shared_ptr<Swapchain>& swapchain)
{
    auto d3d12Device = device->getID3D12Device();
    auto surface = std::shared_ptr<Surface>(new Surface());
    // CommandAllocator
    if (FAILED(d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&surface->m_commandAllocator)))) {
        throw std::runtime_error("failed CreateCommandAllocator()");
    }
    // CommandList
    if (FAILED(d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, surface->m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&surface->m_commandList)))) {
        throw std::runtime_error("failed CreateCommandList()");
    }
    // GBuffer
    D3D12_DESCRIPTOR_HEAP_DESC gHeapDesc = {};
    gHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    gHeapDesc.NodeMask = 0;
    gHeapDesc.NumDescriptors = 3;
    gHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(d3d12Device->CreateDescriptorHeap(&gHeapDesc, IID_PPV_ARGS(&surface->m_gHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    D3D12_CPU_DESCRIPTOR_HANDLE gHandle = surface->m_gHeap->GetCPUDescriptorHandleForHeapStart();
    for (int32_t i = 0; i < 3; i++) {
        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        clearValue.Color[0] = 0.0f;
        clearValue.Color[1] = 0.0f;
        clearValue.Color[2] = 0.0f;
        clearValue.Color[3] = 0.0f;

        D3D12_HEAP_PROPERTIES texHeapProps = {};
        texHeapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
        texHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        texHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        texHeapProps.CreationNodeMask = 0;
        texHeapProps.VisibleNodeMask = 0;
        D3D12_RESOURCE_DESC texResDesc = {};
        texResDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        texResDesc.Width = Screen::getWidth();
        texResDesc.Height = Screen::getHeight();
        texResDesc.DepthOrArraySize = 1;
        texResDesc.SampleDesc.Count = 1;
        texResDesc.SampleDesc.Quality = 0;
        texResDesc.MipLevels = 1;
        texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        texResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        ComPtr<ID3D12Resource>& gTexture = surface->m_gTextures.at(i);
        if (FAILED(d3d12Device->CreateCommittedResource(&texHeapProps, D3D12_HEAP_FLAG_NONE, &texResDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&gTexture)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }

        D3D12_RENDER_TARGET_VIEW_DESC gRtvDesc = {};
        gRtvDesc.Texture2D.MipSlice = 0;
        gRtvDesc.Texture2D.PlaneSlice = 0;
        gRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        gRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        d3d12Device->CreateRenderTargetView(gTexture.Get(), nullptr, gHandle);
        gHandle.ptr += d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    // DepthBuffer
    D3D12_RESOURCE_DESC depthResDesc = {};
    depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResDesc.Width = Screen::getWidth();
    depthResDesc.Height = Screen::getHeight();
    depthResDesc.DepthOrArraySize = 1;
    depthResDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthResDesc.SampleDesc.Count = 1;
    depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    D3D12_HEAP_PROPERTIES depthHeapProps = {};
    depthHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    depthHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    depthHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    D3D12_CLEAR_VALUE depthClearValue = {};
    depthClearValue.DepthStencil.Depth = 1.0f;
    depthClearValue.DepthStencil.Stencil = 0;
    depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (FAILED(d3d12Device->CreateCommittedResource(&depthHeapProps, D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&surface->m_depthBuffer)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    surface->m_depthBuffer->SetName(L"DepthBuffer");
    // DepthStencilViewHeap
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    if (FAILED(d3d12Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&surface->m_depthStencilViewHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    // DepthBufferView
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    d3d12Device->CreateDepthStencilView(surface->m_depthBuffer.Get(), &dsvDesc, surface->m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
    // GlobalLight
    GlobalLight::initialize(d3d12Device, surface->m_gTextures);
    // PointLight
    PointLight::initialize(d3d12Device, surface->m_gTextures);

    surface->m_swapchain = swapchain;
    return surface;
}

void Surface::destroy()
{
    GlobalLight::destroy();
    PointLight::destroy();
    m_swapchain->destroy();
    m_swapchain = nullptr;
}
// private
Surface::Surface()
    : m_swapchain()
    , m_dxgiFactory()
    , m_infoQueue()
    , m_commandAllocator()
    , m_commandList()
    , m_gHeap()
    , m_gTextures(3)
    , m_depthBuffer()
    , m_depthStencilViewHeap()
    , m_fence()
{
}
}