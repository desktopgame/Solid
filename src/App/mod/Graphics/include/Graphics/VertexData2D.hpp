#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexData2D {
    Math::Vector2 position;
    Math::Vector2 texCoord;

    explicit VertexData2D() = default;
    explicit VertexData2D(const Math::Vector2& position, const Math::Vector2& texCoord);
    explicit VertexData2D(float x, float y, float u, float v);
};
}