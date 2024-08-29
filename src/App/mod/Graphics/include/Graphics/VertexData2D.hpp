#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexTexCoord2D {
    Math::Vector2 position;
    Math::Vector2 texCoord;

    explicit VertexTexCoord2D() = default;
    explicit VertexTexCoord2D(const Math::Vector2& position, const Math::Vector2& texCoord);
    explicit VertexTexCoord2D(float x, float y, float u, float v);
};
}