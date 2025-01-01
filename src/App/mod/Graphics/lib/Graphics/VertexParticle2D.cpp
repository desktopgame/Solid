#include <Graphics/VertexParticle2D.hpp>

namespace Lib::Graphics {
VertexParticle2D::VertexParticle2D(const Math::Vector2& offset, const Math::Vector2& velocity, float lifetime)
    : offset(offset)
    , velocity(velocity)
    , lifetime(lifetime)
{
}
VertexParticle2D::VertexParticle2D(float x, float y, float vx, float vy, float lifetime)
    : offset({ x, y })
    , velocity({ vx, vy })
    , lifetime(lifetime)
{
}
}