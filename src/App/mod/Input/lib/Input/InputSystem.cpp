#include <Input/Gamepad.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Keyboard.hpp>
#include <stdexcept>

namespace Lib::Input {
std::mutex InputSystem::s_mutex;
std::shared_ptr<InputSystem> InputSystem::s_instance = nullptr;
// public
std::shared_ptr<InputSystem> InputSystem::getInstance()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        s_instance = std::shared_ptr<InputSystem>(new InputSystem());
    }
    return s_instance;
}

void InputSystem::require()
{
    if (!s_instance || !s_instance->m_started) {
        throw std::logic_error("InputSystem is not exist.");
    }
}

std::shared_ptr<InputSystem> InputSystem::startup(const std::shared_ptr<OS::Window>& window)
{
    // startup runs a only at first call.
    if (m_started) {
        return nullptr;
    }
    m_started = true;
    s_instance->m_keyboard = Keyboard::create(window);
    return s_instance;
}

void InputSystem::sync()
{
    require();
    for (auto gamepad : m_gamepads) {
        if (gamepad) {
            gamepad->sync();
        }
    }
    m_keyboard->sync();
}

void InputSystem::shutdown()
{
    if (!m_started) {
        return;
    }
    if (m_shutdowned) {
        return;
    }
    m_shutdowned = true;
    for (int32_t i = 0; i < 4; i++) {
        m_gamepads.at(i) = nullptr;
    }
    m_keyboard = nullptr;
    s_instance = nullptr;
}

std::shared_ptr<Gamepad> InputSystem::getGamepad(int32_t index)
{
    if (!m_gamepads.at(index)) {
        m_gamepads.at(index) = Gamepad::create(index);
    }
    return m_gamepads.at(index);
}

std::shared_ptr<Keyboard> InputSystem::getKeyboard() const { return m_keyboard; }
// private
InputSystem::InputSystem()
    : m_started(false)
    , m_shutdowned(false)
    , m_gamepads()
{
}
}