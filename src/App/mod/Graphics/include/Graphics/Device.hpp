#pragma once
#include <Graphics/Engine.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace Lib::OS {
class Window;
}

namespace Lib::Graphics {
class Surface;
class Swapchain;
class Device {
public:
    ~Device();

    void flushLogEntries();

    std::shared_ptr<Surface> getSurface() const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Device> create(const std::shared_ptr<OS::Window>& window);
    void destroy();

    Microsoft::WRL::ComPtr<ID3D12Device> getID3D12Device() const;
#endif

private:
    Device();
    void init(const std::shared_ptr<OS::Window>& window);

    std::shared_ptr<Swapchain> m_swapchain;
    std::shared_ptr<Surface> m_surface;

    HWND m_hwnd;
    Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;
};
}