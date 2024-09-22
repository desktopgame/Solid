#pragma once
#include <Input/ButtonState.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <Windows.h>
#endif

namespace Lib::OS {
class Window;
}

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
    Math::IntVector2 getPosition() const;
    Math::IntVector2 getDelta() const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Mouse> create(const std::shared_ptr<OS::Window>& window);

    void handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void sync();
#endif

private:
    explicit Mouse();

    Math::IntVector2 m_delta;
    Math::IntVector2 m_prevPos;
    Math::IntVector2 m_currentPos;
    std::array<ButtonState, (int32_t)Button::Count> m_prevStat;
    std::array<bool, (int32_t)Button::Count> m_currentStat;
    std::shared_ptr<OS::Window> m_window;
};
}