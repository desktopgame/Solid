#pragma once
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <Windows.h>
#endif

namespace Lib::OS {
class Window {
public:
    ~Window();

    void show();
    bool peekMessage() const;
    void hide();

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Window> create(int32_t width, int32_t height);
    void destroy();

    HWND getHWND() const;
#endif

private:
    Window();

#if SOLID_ENABLE_INTERNAL
    HWND m_hwnd;
    WNDCLASSEX m_class;
#endif
};
}