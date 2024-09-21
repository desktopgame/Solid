#pragma once
#include <array>
#include <memory>
#include <mutex>

namespace Lib::OS {
class Window;
}

namespace Lib::Input {
class Gamepad;
class Keyboard;
class InputSystem {
public:
    static std::shared_ptr<InputSystem> getInstance();

    static void require();

    std::shared_ptr<InputSystem> startup(const std::shared_ptr<OS::Window>& window);

    void sync();

    void shutdown();

    std::shared_ptr<Gamepad> getGamepad(int32_t index);

    std::shared_ptr<Keyboard> getKeyboard() const;

private:
    static std::mutex s_mutex;
    static std::shared_ptr<InputSystem> s_instance;
    InputSystem();

    bool m_started;
    bool m_shutdowned;
    std::array<std::shared_ptr<Gamepad>, 4> m_gamepads;
    std::shared_ptr<Keyboard> m_keyboard;
};
}