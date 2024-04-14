#pragma once
#include <Graphics/Device.hpp>
#include <any>
#include <memory>

namespace Lib::Graphics::Internal {
class Swapchain;
}

namespace Lib::Graphics {
class Device;
class Window;
class Surface {
public:
    ~Surface();
    void render();

private:
    Surface();

    std::shared_ptr<Internal::Swapchain> m_swapchain;

    class Impl;
    std::shared_ptr<Impl> m_impl;

    static std::shared_ptr<Surface> create(
        const std::shared_ptr<Device>& device,
        const std::shared_ptr<Internal::Swapchain>& swapchain);
    void destroy();
    // friend std::shared_ptr<Device> Device::create(const std::shared_ptr<Window>& window);
    friend class Lib::Graphics::Device;
};
}