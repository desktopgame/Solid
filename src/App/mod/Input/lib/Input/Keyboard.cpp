#include <Input/Keyboard.hpp>
#include <OS/Window.hpp>

namespace Lib::Input {
// public
Keyboard::~Keyboard() { }

bool Keyboard::isTrigger(KeyCode keyCode) const { return m_prevStat.at((int32_t)keyCode) == k_statTrigger; }
bool Keyboard::isPressed(KeyCode keyCode) const { return m_prevStat.at((int32_t)keyCode) == k_statPressed; }

// internal
std::shared_ptr<Keyboard> Keyboard::create(const std::shared_ptr<OS::Window>& window)
{
    auto keyboard = std::shared_ptr<Keyboard>(new Keyboard());
    window->setCallback(std::bind(&Keyboard::handleEvent, keyboard, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    return keyboard;
}

void Keyboard::sync()
{
    for (int32_t key = 0; key < (int32_t)KeyCode::Count; key++) {
        switch (m_prevStat.at(key)) {
        case k_statNone:
        case k_statReleased:
            if (m_currentStat.at(key)) {
                m_prevStat.at(key) = k_statTrigger;
            }
            break;
        case k_statTrigger:
        case k_statPressed:
            if (m_currentStat.at(key)) {
                m_prevStat.at(key) = k_statPressed;
            } else {
                m_prevStat.at(key) = k_statReleased;
            }
            break;
        }
    }
}
// private
Keyboard::Keyboard()
{
}

void Keyboard::handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_KEYDOWN:
        m_currentStat.at((int32_t)keyMap((int32_t)wparam)) = true;
        break;
    case WM_KEYUP:
        m_currentStat.at((int32_t)keyMap((int32_t)wparam)) = false;
        break;
    }
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