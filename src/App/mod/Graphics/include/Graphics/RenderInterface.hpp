#pragma once
#include <numeric>

namespace Lib::Graphics {
enum class RenderInterface : uint32_t {
    None = 0,
    /**
     * Layout:
     * Matrix = 0
     * Texture = 1
     */
    UseTexture,
    /**
     * Layout:
     * Matrix = 0
     * Color = 1
     */
    UseColor,
    /**
     * Layout:
     * Matrix = 0
     * Texture = 1
     * Color = 2
     */
    UseTextureAndColor,
};
}