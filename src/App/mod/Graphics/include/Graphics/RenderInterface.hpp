#pragma once
#include <numeric>

namespace Lib::Graphics {
class RenderInterface {
public:
    enum Value : uint8_t {
        None = 0,
        /**
         * Layout:
         * Matrix = 0
         * Texture = 1
         */
        Texture,

        /**
         * Layout:
         * Matrix = 0
         * Color = 1
         */
        Color,

        /**
         * Layout:
         * Matrix = 0
         * Texture = 1
         * Color = 2
         */
        TextureAndColor,
    };

    RenderInterface() = default;
    inline constexpr RenderInterface(Value value)
        : m_value(value)
    {
    }

    inline constexpr bool operator==(RenderInterface a) const { return m_value == a.m_value; }
    inline constexpr bool operator!=(RenderInterface a) const { return m_value != a.m_value; }

    inline constexpr bool useTexture() const { return m_value == Texture || m_value == TextureAndColor; }
    inline constexpr bool useColor() const { return m_value == Color || m_value == TextureAndColor; }

private:
    Value m_value;
};
}