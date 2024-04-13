#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Window.hpp>
#include <stdexcept>

namespace Lib::Graphics {
std::mutex Engine::s_mutex;
std::shared_ptr<Engine> Engine::s_instance = nullptr;
Engine::~Engine()
{
    shutdown();
}

std::shared_ptr<Engine> Engine::getInstance()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        s_instance = std::shared_ptr<Engine>(new Engine());
    }
    return s_instance;
}

void Engine::require()
{
    if (!s_instance || !s_instance->m_started) {
        throw std::logic_error("Engine is not exist.");
    }
}

std::shared_ptr<Engine> Engine::startup(int argc, char* argv[])
{
    // startup runs a only at first call.
    if (m_started) {
        return nullptr;
    }
    m_started = true;
    m_window = Window::create(800, 600);
    m_device = Device::create(m_window);
    return s_instance;
}

void Engine::shutdown()
{
    if (!m_started) {
        return;
    }
    s_instance = nullptr;
}
// private
Engine::Engine()
    : m_started(false)
    , m_window()
    , m_device()
{
}
}