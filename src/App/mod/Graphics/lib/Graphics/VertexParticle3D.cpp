#include <Graphics/VertexParticle3D.hpp>

namespace Lib::Graphics {
VertexParticle3D::VertexParticle3D(const Math::Vector3& position, const Math::Vector3& velocity)
    : offset(position)
    , velocity(velocity)
{
}
VertexParticle3D::VertexParticle3D(float x, float y, float z, float vx, float vy, float vz)
    : offset({ x, y, z })
    , velocity({ vx, vy, vz })
{
}
}