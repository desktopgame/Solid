#pragma once
#include <Input/ButtonState.hpp>
#include <array>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <Windows.h>
#endif

namespace Lib::Input {
class Mouse {
public:
    enum Button : int32_t {
        Left = 0,
        Middle,
        Right,
        Count
    };

    ~Mouse();

    ButtonState getState(Button button) const;
    bool isTrigger(Button button) const;
    bool isPressed(Button button) const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Mouse> create();

    void handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void sync();
#endif

private:
    explicit Mouse();

    std::array<ButtonState, (int32_t)Button::Count> m_prevStat;
    std::array<bool, (int32_t)Button::Count> m_currentStat;
};
}