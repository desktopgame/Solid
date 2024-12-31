#pragma once
#include <Math/Vector.hpp>

namespace Lib::Graphics {
struct VertexParticle2D {
    Math::Vector2 offset;
    Math::Vector2 velocity;

    explicit VertexParticle2D() = default;
    explicit VertexParticle2D(const Math::Vector2& position, const Math::Vector2& velocity);
    explicit VertexParticle2D(float x, float y, float vx, float vy);
};
}