// #include <Graphics/Device.hpp>
// #include <Graphics/Window.hpp>
// #include <imgui_internal.h>
#include <OS/Cursor.hpp>
#include <OS/Window.hpp>
#include <tchar.h>

namespace Lib::OS {
static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    auto window = static_cast<Window*>(GetProp(hwnd, _T("UserPtr")));
    if (window) {
        return window->handleEvent(hwnd, msg, wparam, lparam);
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
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
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
std::shared_ptr<Window> Window::create(int32_t width, int32_t height, const std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>& imguiCallback)
{
    auto window = std::shared_ptr<Window>(new Window());
    window->init(width, height, imguiCallback);
    return window;
}

void Window::destroy()
{
    if (m_hwnd != nullptr) {
        DestroyWindow(m_hwnd);
        WNDCLASSEX w = m_class;
        UnregisterClass(w.lpszClassName, w.hInstance);
        m_hwnd = nullptr;
    }
}

LRESULT Window::handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (Cursor::isVisible()) {
        if (m_imguiCallback(hwnd, msg, wparam, lparam)) {
            if (m_userCallback) {
                m_userCallback(hwnd, msg, wparam, lparam);
            }
            return true;
        }
    }
    if (m_userCallback) {
        m_userCallback(hwnd, msg, wparam, lparam);
    }
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::setCallback(const std::function<void(HWND, UINT, WPARAM, LPARAM)>& callback) { m_userCallback = callback; }
std::function<void(HWND, UINT, WPARAM, LPARAM)> Window::getCallback() const { return m_userCallback; }

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

void Window::init(int32_t width, int32_t height, const std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>& imguiCallback)
{
    WNDCLASSEX w = {};
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProcedure;
    w.lpszClassName = _T("DX12");
    w.hInstance = GetModuleHandle(nullptr);
    RegisterClassEx(&w);
    m_class = w;

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
    m_hwnd = hwnd;
    m_imguiCallback = imguiCallback;
    SetProp(hwnd, _T("UserPtr"), this);
}
}