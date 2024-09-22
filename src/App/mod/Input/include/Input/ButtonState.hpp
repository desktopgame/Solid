#pragma once
#include <cstdint>

namespace Lib::Input {
enum class ButtonState : int32_t {
    None = 0,
    Trigger,
    Pressed,
    Released
};
}