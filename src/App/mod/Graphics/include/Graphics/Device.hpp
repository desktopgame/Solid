#pragma once
#include <Graphics/Engine.hpp>
#include <any>
#include <memory>

namespace Lib::Graphics::Internal {
class Swapchain;
}
namespace Lib::Graphics {
class Window;
class Surface;
class Device {
public:
    ~Device();

    void flushLogEntries();
    void render();

    std::any getHandle() const;
    std::shared_ptr<Surface> getSurface() const;

private:
    Device();
    static std::shared_ptr<Device> create(const std::shared_ptr<Window>& window);
    void destroy();

    std::shared_ptr<Internal::Swapchain> m_swapchain;
    std::shared_ptr<Surface> m_surface;

    class Impl;
    std::shared_ptr<Impl> m_impl;

    friend std::shared_ptr<Engine> Engine::startup(int argc, char* argv[]);
    friend void Engine::shutdown();
};
}