#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Swapchain.hpp>
#include <Graphics/VertexData2D.hpp>
#include <Graphics/VertexData3D.hpp>
#include <Math/Vector.hpp>
#include <stdexcept>
#include <vector>

namespace Lib::Graphics {

using Microsoft::WRL::ComPtr;
// public
Surface::~Surface()
{
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
    const std::shared_ptr<RenderParameter>& renderParameter,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength)
{
    renderParameter->update();
    pso->command(m_commandList, renderParameter);

    int32_t vertexComponent = pso->getVertexComponent();
    bool isUsingTexCoord = pso->isUsingTexCoord();
    uint32_t stride = 0;
    if (vertexComponent == 2) {
        if (isUsingTexCoord) {
            stride = sizeof(VertexData2D);
        } else {
            stride = sizeof(Math::Vector2);
        }
    } else if (vertexComponent == 3) {
        if (isUsingTexCoord) {
            stride = sizeof(VertexData3D);
        } else {
            stride = sizeof(Math::Vector3);
        }
    }
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = vertexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    vbView.SizeInBytes = vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(stride);
    m_commandList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = indexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = indexBuffer->getSize();
    m_commandList->IASetIndexBuffer(&ibView);

    m_commandList->DrawIndexedInstanced(indexLength, 1, 0, 0, 0);
}
// internal
std::shared_ptr<Surface> Surface::create(
    const std::shared_ptr<Device>& device,
    const std::shared_ptr<Swapchain>& swapchain)
{
    auto nativeDevice = device->getID3D12Device();
    auto surface = std::shared_ptr<Surface>(new Surface());
    // CommandAllocator
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    if (FAILED(nativeDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) {
        throw std::runtime_error("failed CreateCommandAllocator()");
    }
    surface->m_commandAllocator = commandAllocator;
    // CommandList
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    if (FAILED(nativeDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) {
        throw std::runtime_error("failed CreateCommandList()");
    }
    surface->m_commandList = commandList;
    surface->m_swapchain = swapchain;
    return surface;
}

void Surface::destroy()
{
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