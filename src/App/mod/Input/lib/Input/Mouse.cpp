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
Math::IntVector2 Mouse::getDelta() const { return m_delta; }
// internal
std::shared_ptr<Mouse> Mouse::create(const std::shared_ptr<OS::Window>& window)
{
    auto mouse = std::shared_ptr<Mouse>(new Mouse());
    mouse->m_window = window;
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
    }
    POINT pt;
    if (GetCursorPos(&pt)) {
        if (ScreenToClient(m_window->getHWND(), &pt)) {
            m_currentPos.x() = pt.x;
            m_currentPos.y() = pt.y;
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
    m_delta = m_currentPos - m_prevPos;
    m_prevPos = m_currentPos;

    OS::Cursor::reset();
    POINT pt;
    if (GetCursorPos(&pt)) {
        if (ScreenToClient(m_window->getHWND(), &pt)) {
            m_prevPos.x() = pt.x;
            m_prevPos.y() = pt.y;

            if (!m_isCursorLocked && OS::Cursor::isLocked()) {
                m_currentPos = m_prevPos;
                m_delta = Math::IntVector2({ 0, 0 });
            }
        }
    }
    m_isCursorLocked = OS::Cursor::isLocked();
}
// private
Mouse::Mouse()
    : m_delta({ 0, 0 })
    , m_prevPos({ 0, 0 })
    , m_currentPos({ 0, 0 })
    , m_prevStat()
    , m_currentStat()
    , m_isCursorLocked(false)
    , m_window()
{
}
}