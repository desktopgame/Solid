#pragma once
#include <Windows.h>
#include <functional>
#include <memory>

namespace Lib::OS {
class Window {
public:
    ~Window();

    void show();
    bool peekMessage() const;
    void hide();

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Window> create(int32_t width, int32_t height, const std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>& imguiCallback);
    void destroy();

    LRESULT handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void setCallback(const std::function<void(HWND, UINT, WPARAM, LPARAM)>& callback);
    std::function<void(HWND, UINT, WPARAM, LPARAM)> getCallback() const;

    HWND getHWND() const;
#endif

private:
    Window();

    HWND m_hwnd;
    WNDCLASSEX m_class;
    std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> m_imguiCallback;
    std::function<void(HWND, UINT, WPARAM, LPARAM)> m_userCallback;
};
}