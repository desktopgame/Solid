#include <Graphics/Device.hpp>
#include <Graphics/Window.hpp>
#include <Windows.h>
#include <tchar.h>

namespace Lib::Graphics {
static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
// Impl
class Window::Impl {
public:
    explicit Impl() = default;
    WNDCLASSEX w;
    HWND hwnd;

private:
};
// public
Window::~Window()
{
}

void Window::show()
{
    ShowWindow(m_impl->hwnd, SW_SHOW);
}

bool Window::translateMessage() const
{
    MSG msg = {};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (msg.message == WM_QUIT) {
        return true;
    }
    return false;
}

void Window::hide()
{
    ShowWindow(std::any_cast<HWND>(m_impl->hwnd), SW_HIDE);
}

std::any Window::getHandle() const
{
    return m_impl->hwnd;
}
// private
Window::Window()
    : m_impl(std::make_shared<Impl>())
{
}

std::shared_ptr<Window> Window::create(int32_t width, int32_t height)
{
    auto window = std::shared_ptr<Window>(new Window());
    // create HWND
    WNDCLASSEX w = {};
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProcedure;
    w.lpszClassName = _T("DX12");
    w.hInstance = GetModuleHandle(nullptr);
    RegisterClassEx(&w);
    window->m_impl->w = w;

    RECT wrc = { 0, 0, width, height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    HWND hwnd = CreateWindow(
        w.lpszClassName,
        _T("DX12"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wrc.right - wrc.left,
        wrc.bottom - wrc.top,
        nullptr,
        nullptr,
        w.hInstance,
        nullptr);
    window->m_impl->hwnd = hwnd;
    return window;
}

void Window::destroy()
{
    if (m_impl->hwnd != nullptr) {
        WNDCLASSEX w = m_impl->w;
        UnregisterClass(w.lpszClassName, w.hInstance);
        DestroyWindow(m_impl->hwnd);
        m_impl->hwnd = nullptr;
    }
}
}