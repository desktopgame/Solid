#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Internal/Pso.hpp>
#include <Graphics/Internal/Swapchain.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/VertexData2D.hpp>
#include <Graphics/VertexData3D.hpp>
#include <Math/Vector.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdexcept>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {

using Microsoft::WRL::ComPtr;
// Impl
class Surface::Impl {
public:
    explicit Impl() = default;
    ComPtr<IDXGIFactory6> dxgiFactory;
    ComPtr<ID3D12InfoQueue> infoQueue;
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> commandList;
    ComPtr<ID3D12Fence> fence;

private:
};
// public
Surface::~Surface()
{
}

void Surface::render()
{
}

void Surface::begin()
{
    m_swapchain->clear(m_impl->commandList);
}

void Surface::end()
{
    m_swapchain->swap(m_impl->commandList);
    m_impl->commandList->Close();
    m_swapchain->execute(m_impl->commandList);

    m_swapchain->present(m_impl->commandList);
    m_swapchain->waitSync();

    m_impl->commandAllocator->Reset();
    m_impl->commandList->Reset(m_impl->commandAllocator.Get(), nullptr);
}

void Surface::draw(
    const std::shared_ptr<Shader>& shader,
    const std::shared_ptr<RenderParameter>& renderParameter,
    PrimitiveType primitiveType,
    int32_t vertexComponent,
    bool isUsingTexCoord,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength)
{
    if (!m_pso) {
        // TODO: rent from pool.
        m_pso = Internal::Pso::create(shader, renderParameter, primitiveType, vertexComponent, isUsingTexCoord);
    }
    m_pso->update();
    m_pso->command(m_impl->commandList);
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
    vbView.BufferLocation = vertexBuffer->getVirtualAddress();
    vbView.SizeInBytes = vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(stride);
    m_impl->commandList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = indexBuffer->getVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = indexBuffer->getSize();
    m_impl->commandList->IASetIndexBuffer(&ibView);

    m_impl->commandList->DrawIndexedInstanced(indexLength, 1, 0, 0, 0);
}
// private
Surface::Surface()
    : m_swapchain()
    , m_impl(std::make_shared<Impl>())
{
}

std::shared_ptr<Surface> Surface::create(
    const std::shared_ptr<Device>& device,
    const std::shared_ptr<Internal::Swapchain>& swapchain)
{
    auto nativeDevice = std::any_cast<ComPtr<ID3D12Device>>(device->getHandle());
    auto surface = std::shared_ptr<Surface>(new Surface());
    // CommandAllocator
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    if (FAILED(nativeDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) {
        throw std::runtime_error("failed CreateCommandAllocator()");
    }
    surface->m_impl->commandAllocator = commandAllocator;
    // CommandList
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    if (FAILED(nativeDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) {
        throw std::runtime_error("failed CreateCommandList()");
    }
    surface->m_impl->commandList = commandList;
    surface->m_swapchain = swapchain;
    return surface;
}

void Surface::destroy()
{
}
}