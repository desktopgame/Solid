#include <Input/Mouse.hpp>
#include <OS/Cursor.hpp>
#include <OS/Window.hpp>

namespace Lib::Input {
// public
Mouse::~Mouse() { }

ButtonState Mouse::getState(Mouse::Button button) const
{
    if (isTrigger(button)) {
        return ButtonState::Trigger;
    }
    return isPressed(button) ? ButtonState::Pressed : ButtonState::Released;
}
bool Mouse::isTrigger(Mouse::Button button) const { return m_prevStat.at((int32_t)button) == ButtonState::Trigger; }
bool Mouse::isPressed(Mouse::Button button) const { return m_prevStat.at((int32_t)button) == ButtonState::Pressed; }
Math::IntVector2 Mouse::getPosition() const { return m_currentPos; }
Math::IntVector2 Mouse::getDelta() const { return m_delta2; }
// internal
std::shared_ptr<Mouse> Mouse::create(const std::shared_ptr<OS::Window>& window)
{
    auto mouse = std::shared_ptr<Mouse>(new Mouse());
    mouse->m_window = window;

    std::vector<RAWINPUTDEVICE> rid;
    rid.push_back({});
    rid.at(0).usUsagePage = 0x01;
    rid.at(0).usUsage = 0x02;
    rid.at(0).dwFlags = 0;
    rid.at(0).hwndTarget = window->getHWND();
    RegisterRawInputDevices(rid.data(), rid.size(), sizeof(RAWINPUTDEVICE));
    return mouse;
}

void Mouse::handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_LBUTTONDOWN:
        m_currentStat.at((int32_t)Mouse::Button::Left) = true;
        break;
    case WM_LBUTTONUP:
        m_currentStat.at((int32_t)Mouse::Button::Left) = false;
        break;
    case WM_MBUTTONDOWN:
        m_currentStat.at((int32_t)Mouse::Button::Middle) = true;
        break;
    case WM_MBUTTONUP:
        m_currentStat.at((int32_t)Mouse::Button::Middle) = false;
        break;
    case WM_RBUTTONDOWN:
        m_currentStat.at((int32_t)Mouse::Button::Right) = true;
        break;
    case WM_RBUTTONUP:
        m_currentStat.at((int32_t)Mouse::Button::Right) = false;
        break;
    case WM_INPUT:
        UINT dataSize;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));

        if (dataSize > 0) {
            std::vector<BYTE> buffer(dataSize);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, buffer.data(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
                RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(buffer.data());

                if (rawInput->header.dwType == RIM_TYPEMOUSE) {
                    m_delta.x() += rawInput->data.mouse.lLastX;
                    m_delta.y() += rawInput->data.mouse.lLastY;
                }
            }
        }
        break;
    }
    POINT pt;
    POINT save;
    m_prevPos = m_currentPos;
    if (GetCursorPos(&pt)) {
        save = pt;
        if (ScreenToClient(m_window->getHWND(), &pt)) {
            m_currentPos.x() = pt.x;
            m_currentPos.y() = pt.y;
        }
        if (OS::Cursor::isLocked()) {
            // SetCursorPos(save.x, save.y);
        }
    }
}

void Mouse::sync()
{
    for (int32_t key = 0; key < (int32_t)Mouse::Button::Count; key++) {
        switch (m_prevStat.at(key)) {
        case ButtonState::None:
        case ButtonState::Released:
            if (m_currentStat.at(key)) {
                m_prevStat.at(key) = ButtonState::Trigger;
            }
            break;
        case ButtonState::Trigger:
        case ButtonState::Pressed:
            if (m_currentStat.at(key)) {
                m_prevStat.at(key) = ButtonState::Pressed;
            } else {
                m_prevStat.at(key) = ButtonState::Released;
            }
            break;
        }
    }
    m_delta2 = m_delta;
    m_delta = Math::IntVector2({ 0, 0 });
}
// private
Mouse::Mouse()
    : m_delta({ 0, 0 })
    , m_delta2({ 0, 0 })
    , m_currentPos({ 0, 0 })
    , m_prevStat()
    , m_currentStat()
    , m_window()
{
}
}