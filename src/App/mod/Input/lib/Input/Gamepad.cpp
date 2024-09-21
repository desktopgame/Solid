#include <Input/Gamepad.hpp>
#include <Windows.h>
#include <Xinput.h>
#include <array>
#include <iostream>

namespace Lib::Input {
// Impl
class Gamepad::Impl {
public:
    explicit Impl() = default;
    XINPUT_STATE prev;
    XINPUT_STATE stat;
};
// public
uint8_t Gamepad::getLeftTrigger() const { return static_cast<uint8_t>(m_impl->stat.Gamepad.bLeftTrigger); }
uint8_t Gamepad::getRightTrigger() const { return static_cast<uint8_t>(m_impl->stat.Gamepad.bRightTrigger); }
int16_t Gamepad::getLeftStickX() const { return static_cast<int16_t>(m_impl->stat.Gamepad.sThumbLX); }
int16_t Gamepad::getLeftStickY() const { return static_cast<int16_t>(m_impl->stat.Gamepad.sThumbLY); }
int16_t Gamepad::getRightStickX() const { return static_cast<int16_t>(m_impl->stat.Gamepad.sThumbRX); }
int16_t Gamepad::getRightStickY() const { return static_cast<int16_t>(m_impl->stat.Gamepad.sThumbRY); }
ButtonState Gamepad::getState(Button button) const
{
    if (isTrigger(button)) {
        return ButtonState::Trigger;
    }
    return isPressed(button) ? ButtonState::Pressed : ButtonState::Released;
}
bool Gamepad::isTrigger(Button button) const { return (m_impl->prev.Gamepad.wButtons & button) == 0 && (m_impl->stat.Gamepad.wButtons & button) > 0; }
bool Gamepad::isPressed(Button button) const { return (m_impl->stat.Gamepad.wButtons & button) > 0; }
bool Gamepad::isEnabled() const { return m_isEnabled; }
// internal
void Gamepad::sync()
{
    if (!m_impl) {
        m_impl = std::make_shared<Impl>();
    }
    m_impl->prev = m_impl->stat;
    DWORD result = XInputGetState(m_index, &m_impl->stat);
    if (result == ERROR_SUCCESS) {
        m_isEnabled = true;
    } else {
        m_isEnabled = false;
    }
}

std::shared_ptr<Gamepad> Gamepad::create(int32_t index)
{
    return std::shared_ptr<Gamepad>(new Gamepad(index));
}

// private
Gamepad::Gamepad(int32_t index)
    : m_index(index)
    , m_isEnabled(false)
    , m_impl(std::shared_ptr<Impl>(new Impl()))
{
}
}