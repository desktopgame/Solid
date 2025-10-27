#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
/**
 * フォントのサイズ、整列方法を保持するクラスです。
 */
class FontMetrics {
public:
    explicit FontMetrics(Math::IntVector2 size, Math::IntVector2 bearing,
        Math::IntVector2 advance);

    const Math::IntVector2 size;
    const Math::IntVector2 bearing;
    const Math::IntVector2 advance;
};
}
