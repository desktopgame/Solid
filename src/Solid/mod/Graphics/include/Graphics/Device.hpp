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

private:
    Device();
    static std::shared_ptr<Device> create(const std::shared_ptr<Window>& window);

    class Impl;
    std::shared_ptr<Impl> m_impl;

    friend std::shared_ptr<Engine> Engine::startup(int argc, char* argv[]);
};
}