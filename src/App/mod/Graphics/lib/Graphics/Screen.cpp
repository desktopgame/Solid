#include <Graphics/Screen.hpp>

namespace Lib::Graphics {
int32_t Screen::s_width = 800;
int32_t Screen::s_height = 600;

void Screen::setSize(const Math::IntVector2& size) { setSize(size.x(), size.y()); }
Math::IntVector2 Screen::getSize()
{
    return Math::IntVector2({ s_width, s_height });
}

void Screen::setSize(int32_t width, int32_t height)
{
    s_width = width;
    s_height = height;
}
int32_t Screen::getWidth() { return s_width; }
int32_t Screen::getHeight() { return s_height; }

float Screen::getAspectRatio() { return static_cast<float>(s_width) / static_cast<float>(s_height); }
}