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
// public
Window::~Window()
{
}

std::any Window::getHandle() const
{
    return m_hwnd;
}
// private
Window::Window()
    : m_hwnd()
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
    window->m_hwnd = hwnd;
    return window;
}
}