#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Window.hpp>
#include <Windows.h>
#include <imgui.h>
#include <stdexcept>

namespace Lib::Graphics {
std::mutex Engine::s_mutex;
std::shared_ptr<Engine> Engine::s_instance = nullptr;
// public
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
    CoInitializeEx(0, COINIT_MULTITHREADED);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    m_window = Window::create(Screen::getWidth(), Screen::getHeight());
    m_device = Device::create(m_window);
    return s_instance;
}

void Engine::shutdown()
{
    if (!m_started) {
        return;
    }
    if (m_shutdowned) {
        return;
    }
    m_shutdowned = true;

    m_window->destroy();
    m_window = nullptr;
    m_device->destroy();
    m_device = nullptr;
    // ImGui
    ImGui::DestroyContext();
    s_instance = nullptr;
}

std::shared_ptr<Window> Engine::getWindow() const
{
    require();
    return m_window;
}

std::shared_ptr<Device> Engine::getDevice() const
{
    require();
    return m_device;
}
// private
Engine::Engine()
    : m_started(false)
    , m_shutdowned(false)
    , m_window()
    , m_device()
{
}
}