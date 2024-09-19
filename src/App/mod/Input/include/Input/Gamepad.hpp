#pragma once
#include <memory>
#include <vector>

namespace Lib::Input {
class Gamepad {
public:
    enum Button : uint16_t {
        DPadUp = 0x001,
        DPadDown = 0x002,
        DPadLeft = 0x004,
        DPadRight = 0x008,
        Start = 0x0010,
        Back = 0x0020,
        LeftStick = 0x0040,
        RightStick = 0x0080,
        LeftSholder = 0x0100,
        RightSholder = 0x0200,
        A = 0x1000,
        B = 0x2000,
        X = 0x4000,
        Y = 0x8000
    };

    uint8_t getLeftTrigger() const;
    uint8_t getRightTrigger() const;
    int16_t getLeftStickX() const;
    int16_t getLeftStickY() const;
    int16_t getRightStickX() const;
    int16_t getRightStickY() const;
    bool isTrigger(Button button) const;
    bool isPressed(Button button) const;
    bool isEnabled() const;

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Gamepad> create(int32_t index);

    void sync();
#endif

private:
    Gamepad(int32_t index);

    int32_t m_index;
    bool m_isEnabled;

    class Impl;
    std::shared_ptr<Impl> m_impl;
};
}