#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexNormalTexCoord3D {
    Math::Vector3 position;
    Math::Vector3 normal;
    Math::Vector2 texCoord;

    explicit VertexNormalTexCoord3D() = default;
    explicit VertexNormalTexCoord3D(const Math::Vector3& position, const Math::Vector3& normal, const Math::Vector2& texCoord);
};
}