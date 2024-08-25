#pragma once
#include <Math/Vector.hpp>
#include <numeric>

namespace Lib::Graphics {
class Screen {
public:
    static void setSize(const Math::IntVector2& size);
    static Math::IntVector2 getSize();

    static void setSize(int32_t width, int32_t height);
    static int32_t getWidth();
    static int32_t getHeight();

    static float getAspectRatio();

private:
    static int32_t s_width;
    static int32_t s_height;

    Screen() = delete;
    ~Screen() = delete;
};
}