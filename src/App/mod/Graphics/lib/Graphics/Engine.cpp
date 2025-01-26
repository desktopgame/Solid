#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/FontFactory.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/UniformPool.hpp>
#include <OS/Window.hpp>
#include <Utils/Clock.hpp>
#include <imgui.h>
#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

    m_window = OS::Window::create(Screen::getWidth(), Screen::getHeight(), ImGui_ImplWin32_WndProcHandler);
    m_device = Device::create(m_window);
    RenderContext::initialize();
    UniformPool::initialize();
    Utils::Clock::initialize();
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

    Utils::Clock::destroy();
    UniformPool::destroy();
    RenderContext::destroy();
    FontFactory::destroy();

    m_window->destroy();
    m_window = nullptr;
    m_device->destroy();
    m_device = nullptr;
    // ImGui
    ImGui::DestroyContext();
    s_instance = nullptr;
}

std::shared_ptr<OS::Window> Engine::getWindow() const
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