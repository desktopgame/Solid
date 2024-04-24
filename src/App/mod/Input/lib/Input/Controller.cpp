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
    std::array<XINPUT_STATE, Gamepad::k_maxCount> stats;
};
std::shared_ptr<Gamepad::Impl> Gamepad::s_impl = nullptr;
std::vector<std::shared_ptr<Gamepad>> Gamepad::s_controllers(Gamepad::k_maxCount);
// public
void Gamepad::sync()
{
    if (!s_impl) {
        s_impl = std::make_shared<Impl>();
    }
    for (int32_t i = 0; i < k_maxCount; i++) {
        DWORD result = XInputGetState(i, &s_impl->stats.at(i));
        if (result == ERROR_SUCCESS) {
            getGamepad(i)->m_isEnabled = true;
        } else {
            getGamepad(i)->m_isEnabled = false;
        }
    }
}

std::shared_ptr<Gamepad> Gamepad::getGamepad(int32_t index)
{
    if (index < 0 || index >= k_maxCount) {
        throw std::logic_error("unexpected range.");
    }
    if (!s_controllers.at(index)) {
        s_controllers[index] = std::shared_ptr<Gamepad>(new Gamepad(index));
    }
    return s_controllers.at(index);
}

uint8_t Gamepad::getLeftTrigger() const { return static_cast<uint8_t>(s_impl->stats[m_index].Gamepad.bLeftTrigger); }
uint8_t Gamepad::getRightTrigger() const { return static_cast<uint8_t>(s_impl->stats[m_index].Gamepad.bRightTrigger); }
int16_t Gamepad::getLeftStickX() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbLX); }
int16_t Gamepad::getLeftStickY() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbLY); }
int16_t Gamepad::getRightStickX() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbRX); }
int16_t Gamepad::getRightStickY() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbRY); }
bool Gamepad::isPressed(Button button) const { return (s_impl->stats[m_index].Gamepad.wButtons & button) > 0; }
bool Gamepad::isEnabled() const { return m_isEnabled; }
// private
Gamepad::Gamepad(int32_t index)
    : m_index(index)
    , m_isEnabled(false)
{
}
}