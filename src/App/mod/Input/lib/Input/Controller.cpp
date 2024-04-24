#include <Input/Controller.hpp>
#include <Windows.h>
#include <Xinput.h>
#include <array>
#include <iostream>

namespace Lib::Input {
// Impl
class Controller::Impl {
public:
    explicit Impl() = default;
    std::array<XINPUT_STATE, Controller::k_maxCount> stats;
};
std::shared_ptr<Controller::Impl> Controller::s_impl = nullptr;
std::vector<std::shared_ptr<Controller>> Controller::s_controllers(Controller::k_maxCount);
// public
void Controller::sync()
{
    if (!s_impl) {
        s_impl = std::make_shared<Impl>();
    }
    for (int32_t i = 0; i < k_maxCount; i++) {
        DWORD result = XInputGetState(i, &s_impl->stats.at(i));
        if (result == ERROR_SUCCESS) {
            getController(i)->m_isEnabled = true;
        } else {
            getController(i)->m_isEnabled = false;
        }
    }
}

std::shared_ptr<Controller> Controller::getController(int32_t index)
{
    if (index < 0 || index >= k_maxCount) {
        throw std::logic_error("unexpected range.");
    }
    if (!s_controllers.at(index)) {
        s_controllers[index] = std::shared_ptr<Controller>(new Controller(index));
    }
    return s_controllers.at(index);
}

uint8_t Controller::getLeftTrigger() const { return static_cast<uint8_t>(s_impl->stats[m_index].Gamepad.bLeftTrigger); }
uint8_t Controller::getRightTrigger() const { return static_cast<uint8_t>(s_impl->stats[m_index].Gamepad.bRightTrigger); }
int16_t Controller::getLeftStickX() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbLX); }
int16_t Controller::getLeftStickY() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbLY); }
int16_t Controller::getRightStickX() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbRX); }
int16_t Controller::getRightStickY() const { return static_cast<int16_t>(s_impl->stats[m_index].Gamepad.sThumbRY); }
bool Controller::isPressed(Button button) const { return (s_impl->stats[m_index].Gamepad.wButtons & button) > 0; }
bool Controller::isEnabled() const { return m_isEnabled; }
// private
Controller::Controller(int32_t index)
    : m_index(index)
    , m_isEnabled(false)
{
}
}