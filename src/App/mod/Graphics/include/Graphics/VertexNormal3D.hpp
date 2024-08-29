#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexNormal3D {
    Math::Vector3 position;
    Math::Vector3 normal;

    explicit VertexNormal3D() = default;
    explicit VertexNormal3D(const Math::Vector3& position, const Math::Vector3& normal);
};
}