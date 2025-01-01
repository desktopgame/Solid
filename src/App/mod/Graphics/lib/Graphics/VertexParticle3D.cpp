#include <Graphics/VertexParticle3D.hpp>

namespace Lib::Graphics {
VertexParticle3D::VertexParticle3D(const Math::Vector3& offset, const Math::Vector3& velocity, float lifetime)
    : offset(offset)
    , velocity(velocity)
    , lifetime(lifetime)
{
}
VertexParticle3D::VertexParticle3D(float x, float y, float z, float vx, float vy, float vz, float lifetime)
    : offset({ x, y, z })
    , velocity({ vx, vy, vz })
    , lifetime(lifetime)
{
}
}