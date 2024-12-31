#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexParticle3D {
    Math::Vector3 offset;
    Math::Vector3 velocity;

    explicit VertexParticle3D() = default;
    explicit VertexParticle3D(const Math::Vector3& position, const Math::Vector3& velocity);
    explicit VertexParticle3D(float x, float y, float z, float vx, float vy, float vz);
};
}