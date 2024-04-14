#include <Graphics/Device.hpp>
#include <Graphics/Internal/Swapchain.hpp>
#include <Graphics/Surface.hpp>
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
    // Main loop
    m_impl->commandAllocator->Reset();

    m_swapchain->begin(m_impl->commandList);

    m_impl->commandList->Close();

    m_swapchain->end(m_impl->commandList);

    m_impl->commandAllocator->Reset();
    m_impl->commandList->Reset(m_impl->commandAllocator.Get(), nullptr);
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