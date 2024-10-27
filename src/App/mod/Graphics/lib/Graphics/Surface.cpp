#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/PipelineStateObject.hpp>
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

void Surface::begin()
{
    m_swapchain->clear(m_commandList);
}

void Surface::end()
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
    surface->m_swapchain = swapchain;
    return surface;
}

void Surface::destroy()
{
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
    , m_fence()
{
}
}