#include <Graphics/Device.hpp>
#include <Graphics/Window.hpp>
#include <imgui_internal.h>
#include <tchar.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Lib::Graphics {
static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return true;
    }
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

void Window::show()
{
    ShowWindow(m_hwnd, SW_SHOW);
}

bool Window::peekMessage() const
{
    MSG msg = {};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (msg.message == WM_QUIT) {
        return false;
    }
    return true;
}

void Window::hide()
{
    ShowWindow(m_hwnd, SW_HIDE);
}
// internal
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
    window->m_class = w;

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

void Window::destroy()
{
    if (m_hwnd != nullptr) {
        WNDCLASSEX w = m_class;
        UnregisterClass(w.lpszClassName, w.hInstance);
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

HWND Window::getHWND() const
{
    return m_hwnd;
}
// private
Window::Window()
    : m_hwnd(nullptr)
    , m_class()
{
}
}