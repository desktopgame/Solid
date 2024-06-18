#include <Graphics/Device.hpp>
#include <Graphics/Internal/Swapchain.hpp>
#include <Graphics/Surface.hpp>
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

private:
};
// public
Device::~Device()
{
}

void Device::flushLogEntries()
{
#if _DEBUG
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
#endif
}

std::any Device::getHandle() const
{
    return m_impl->device;
}

std::shared_ptr<Surface> Device::getSurface() const
{
    return m_surface;
}
// private
Device::Device()
    : m_swapchain()
    , m_surface()
    , m_impl(std::make_shared<Impl>())
{
}

std::shared_ptr<Device> Device::create(const std::shared_ptr<Window>& window)
{
    std::any hwnd = window->getHandle();
    auto device = std::shared_ptr<Device>(new Device());
    device->m_impl->hwnd = std::any_cast<HWND>(hwnd);
    // Debug Layer
#if _DEBUG
    ComPtr<ID3D12Debug> debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
#endif
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
#if _DEBUG
    ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
    if (FAILED(nativeDevice->QueryInterface(__uuidof(ID3D12InfoQueue), &infoQueue))) {
        throw std::runtime_error("failed ID3D12InfoQueue()");
    }
    device->m_impl->infoQueue = infoQueue;
#endif
    // Swapchain
    device->m_swapchain = Internal::Swapchain::create(window, device->m_impl->dxgiFactory, nativeDevice);
    // Surface
    device->m_surface = Surface::create(device, device->m_swapchain);
    return device;
}

void Device::destroy()
{
    m_impl = nullptr;
}
}