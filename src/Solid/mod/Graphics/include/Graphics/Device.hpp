#pragma once
#include <Graphics/Engine.hpp>
#include <any>
#include <memory>

namespace Lib::Graphics {
class Window;
class Device {
public:
    ~Device();

    void flushLogEntries();
    void render();

private:
    Device();
    static std::shared_ptr<Device> create(const std::shared_ptr<Window>& window);
    void destroy();

    uint64_t m_fenceVal;

    class Impl;
    std::shared_ptr<Impl> m_impl;

    friend std::shared_ptr<Engine> Engine::startup(int argc, char* argv[]);
    friend void Engine::shutdown();
};
}