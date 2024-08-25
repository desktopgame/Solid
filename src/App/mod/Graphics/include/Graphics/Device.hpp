#pragma once
#include <Graphics/Engine.hpp>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Window;
class Surface;
class Swapchain;
class Device {
public:
    ~Device();

    void flushLogEntries();

    std::shared_ptr<Surface> getSurface() const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Device> create(const std::shared_ptr<Window>& window);
    void destroy();

    Microsoft::WRL::ComPtr<ID3D12Device> getID3D12Device() const;
#endif

private:
    Device();

    std::shared_ptr<Swapchain> m_swapchain;
    std::shared_ptr<Surface> m_surface;

#if SOLID_ENABLE_INTERNAL
    HWND m_hwnd;
    Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;
#endif
};
}