#include <Input/Keyboard.hpp>

namespace Lib::Input {
// public
Keyboard::~Keyboard() { }

ButtonState Keyboard::getState(KeyCode keyCode) const
{
    if (isTrigger(keyCode)) {
        return ButtonState::Trigger;
    }
    return isPressed(keyCode) ? ButtonState::Pressed : ButtonState::Released;
}
bool Keyboard::isTrigger(KeyCode keyCode) const { return m_prevStat.at((int32_t)keyCode) == ButtonState::Trigger; }
bool Keyboard::isPressed(KeyCode keyCode) const { return m_prevStat.at((int32_t)keyCode) == ButtonState::Pressed; }

// internal
std::shared_ptr<Keyboard> Keyboard::create()
{
    auto keyboard = std::shared_ptr<Keyboard>(new Keyboard());
    return keyboard;
}

void Keyboard::handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_KEYDOWN: {
        int32_t index = (int32_t)keyMap((int32_t)wparam);
        if (index >= 0) {
            m_currentStat.at(index) = true;
        }
    } break;
    case WM_KEYUP: {
        int32_t index = (int32_t)keyMap((int32_t)wparam);
        if (index >= 0) {
            m_currentStat.at(index) = false;
        }
    } break;
    }
}

void Keyboard::sync()
{
    for (int32_t key = 0; key < (int32_t)KeyCode::Count; key++) {
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
Keyboard::Keyboard()
    : m_prevStat()
    , m_currentStat()
{
}

KeyCode Keyboard::keyMap(int32_t key)
{
    switch (key) {
    case 'W':
        return KeyCode::W;
    case 'A':
        return KeyCode::A;
    case 'S':
        return KeyCode::S;
    case 'D':
        return KeyCode::D;
    case VK_SPACE:
        return KeyCode::Space;
    case VK_SHIFT:
        return KeyCode::Shift;
    case VK_UP:
        return KeyCode::ArrowUp;
    case VK_DOWN:
        return KeyCode::ArrowDown;
    case VK_LEFT:
        return KeyCode::ArrowLeft;
    case VK_RIGHT:
        return KeyCode::ArrowRight;
    }
    return (KeyCode)-1;
}
}