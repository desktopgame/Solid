#pragma once
#include <Input/ButtonState.hpp>
#include <array>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <Windows.h>
#endif

namespace Lib::Input {
enum class KeyCode : int32_t {
    W = 0,
    A,
    S,
    D,
    E,
    Esc,
    Space,
    Shift,
    ArrowUp,
    ArrowDown,
    ArrowLeft,
    ArrowRight,
    Count
};

class Keyboard {
public:
    ~Keyboard();

    ButtonState getState(KeyCode keyCode) const;
    bool isTrigger(KeyCode keyCode) const;
    bool isPressed(KeyCode keyCode) const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Keyboard> create();

    void handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void sync();
#endif

private:
    explicit Keyboard();
    static KeyCode keyMap(int32_t key);

    std::array<ButtonState, (int32_t)KeyCode::Count> m_prevStat;
    std::array<bool, (int32_t)KeyCode::Count> m_currentStat;
};
}