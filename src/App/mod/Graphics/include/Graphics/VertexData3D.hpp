#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexData3D {
    Math::Vector3 position;
    Math::Vector2 texCoord;

    explicit VertexData3D() = default;
    explicit VertexData3D(const Math::Vector3& position, const Math::Vector2& texCoord);
    explicit VertexData3D(float x, float y, float z, float u, float v);
};
}