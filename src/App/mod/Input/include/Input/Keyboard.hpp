#pragma once
#include <array>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <Windows.h>
#endif

namespace Lib::OS {
class Window;
}

namespace Lib::Input {
enum class KeyCode : int32_t {
    W = 0,
    A,
    S,
    D,
    Space,
    ArrowUp,
    ArrowDown,
    ArrowLeft,
    ArrowRight,
    Count
};

class Keyboard {
public:
    ~Keyboard();

    bool isTrigger(KeyCode keyCode) const;
    bool isPressed(KeyCode keyCode) const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Keyboard> create(const std::shared_ptr<OS::Window>& window);

    void sync();
#endif

private:
    explicit Keyboard();

#if SOLID_ENABLE_INTERNAL
    void handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static KeyCode keyMap(int32_t key);
#endif
    inline static const int32_t k_statNone = 0;
    inline static const int32_t k_statTrigger = 1;
    inline static const int32_t k_statPressed = 2;
    inline static const int32_t k_statReleased = 3;

    std::array<int32_t, (int32_t)KeyCode::Count> m_prevStat;
    std::array<bool, (int32_t)KeyCode::Count> m_currentStat;
};
}