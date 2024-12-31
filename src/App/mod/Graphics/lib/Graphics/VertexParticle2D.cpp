#include <Graphics/VertexParticle2D.hpp>

namespace Lib::Graphics {
VertexParticle2D::VertexParticle2D(const Math::Vector2& position, const Math::Vector2& velocity)
    : offset(position)
    , velocity(velocity)
{
}
VertexParticle2D::VertexParticle2D(float x, float y, float vx, float vy)
    : offset({ x, y })
    , velocity({ vx, vy })
{
}
}