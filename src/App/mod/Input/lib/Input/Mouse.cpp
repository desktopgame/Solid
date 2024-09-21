#include <Input/Mouse.hpp>

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

// internal
std::shared_ptr<Mouse> Mouse::create()
{
    auto mouse = std::shared_ptr<Mouse>(new Mouse());
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
}
// private
Mouse::Mouse() { }
}