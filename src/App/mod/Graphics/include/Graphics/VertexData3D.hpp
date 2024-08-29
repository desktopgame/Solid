#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexTexCoord3D {
    Math::Vector3 position;
    Math::Vector2 texCoord;

    explicit VertexTexCoord3D() = default;
    explicit VertexTexCoord3D(const Math::Vector3& position, const Math::Vector2& texCoord);
    explicit VertexTexCoord3D(float x, float y, float z, float u, float v);
};
}