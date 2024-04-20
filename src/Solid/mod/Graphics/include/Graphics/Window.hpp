#pragma once
#include <Graphics/Engine.hpp>
#include <Graphics/Window.hpp>
#include <any>
#include <memory>

namespace Lib::Graphics {
class Window {
public:
    ~Window();

    void show();
    bool translateMessage() const;
    void hide();

    std::any getHandle() const;

private:
    Window();

    class Impl;
    std::shared_ptr<Impl> m_impl;

    static std::shared_ptr<Window> create(int32_t width, int32_t height);
    void destroy();

    friend std::shared_ptr<Engine> Engine::startup(int argc, char* argv[]);
    friend void Engine::shutdown();
};
}