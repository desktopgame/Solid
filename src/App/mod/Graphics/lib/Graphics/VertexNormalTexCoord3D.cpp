#include <Graphics/VertexNormalTexCoord3D.hpp>

namespace Lib::Graphics {
VertexNormalTexCoord3D::VertexNormalTexCoord3D(const Math::Vector3& position, const Math::Vector3& normal, const Math::Vector2& texCoord)
    : position(position)
    , normal(normal)
    , texCoord(texCoord)
{
}
}