#include <Graphics/Device.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Swapchain.hpp>
#include <OS/Window.hpp>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
Device::~Device()
{
}

void Device::flushLogEntries()
{
#if _DEBUG
    // Show messages
    ComPtr<ID3D12InfoQueue> infoQueue = m_infoQueue;
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

std::shared_ptr<Surface> Device::getSurface() const
{
    return m_surface;
}
// internal
std::shared_ptr<Device> Device::create(const std::shared_ptr<OS::Window>& window)
{
    auto device = std::shared_ptr<Device>(new Device());
    device->m_hwnd = window->getHWND();
    // Debug Layer
#if _DEBUG
    ComPtr<ID3D12Debug> debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
#endif
    // Factory
    UINT flags = DXGI_CREATE_FACTORY_DEBUG;
    if (FAILED(CreateDXGIFactory2(flags, IID_PPV_ARGS(&device->m_dxgiFactory)))) {
        throw std::runtime_error("failed CreateDXGIFactory2()");
    }
    // Adapter
    std::vector<ComPtr<IDXGIAdapter>> adapters;
    ComPtr<IDXGIAdapter> mainAdapter = nullptr;
    for (uint32_t i = 0; device->m_dxgiFactory->EnumAdapters(i, &mainAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
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
    for (D3D_FEATURE_LEVEL level : levels) {
        HRESULT deviceStatus = D3D12CreateDevice(mainAdapter.Get(), level, IID_PPV_ARGS(&device->m_device));
        if (SUCCEEDED(deviceStatus)) {
            useLevel = level;
            break;
        }
    }
    if (!useLevel) {
        throw std::runtime_error("failed D3D12CreateDevice()");
    }
    // InfoQueue
#if _DEBUG
    if (FAILED(device->m_device->QueryInterface(__uuidof(ID3D12InfoQueue), &device->m_infoQueue))) {
        throw std::runtime_error("failed ID3D12InfoQueue()");
    }
#endif
    // Swapchain
    device->m_swapchain = Swapchain::create(window, device->m_dxgiFactory, device->m_device);
    // Surface
    device->m_surface = Surface::create(device, device->m_swapchain);
    return device;
}

void Device::destroy()
{
    m_surface->destroy();
    m_surface = nullptr;

    m_swapchain->destroy();
    m_swapchain = nullptr;
}

Microsoft::WRL::ComPtr<ID3D12Device> Device::getID3D12Device() const
{
    return m_device;
}
// private
Device::Device()
    : m_swapchain()
    , m_surface()
    , m_hwnd(nullptr)
    , m_dxgiFactory()
    , m_device()
    , m_infoQueue()
{
}
}