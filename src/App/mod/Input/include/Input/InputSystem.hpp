#pragma once
#include <array>
#include <memory>
#include <mutex>

namespace Lib::Input {
class Gamepad;
class InputSystem {
public:
    static std::shared_ptr<InputSystem> getInstance();

    static void require();

    std::shared_ptr<InputSystem> startup();

    void sync();

    void shutdown();

    std::shared_ptr<Gamepad> getGamepad(int32_t index);

private:
    static std::mutex s_mutex;
    static std::shared_ptr<InputSystem> s_instance;
    InputSystem();

    bool m_started;
    bool m_shutdowned;
    std::array<std::shared_ptr<Gamepad>, 4> m_gamepads;
};
}