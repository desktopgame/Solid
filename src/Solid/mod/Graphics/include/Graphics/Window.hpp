#pragma once
#include <Graphics/Engine.hpp>
#include <Graphics/Window.hpp>
#include <any>
#include <memory>

namespace Lib::Graphics {
class Window {
public:
    ~Window();
    std::any getHandle() const;

private:
    Window();
    std::any m_hwnd;

    static std::shared_ptr<Window> create(int32_t width, int32_t height);

    friend std::shared_ptr<Engine> Engine::startup(int argc, char* argv[]);
};
}