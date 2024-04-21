#include <Graphics/VertexData3D.hpp>

namespace Lib::Graphics {
VertexData3D::VertexData3D(const Math::Vector3& position, const Math::Vector2& texCoord)
    : position(position)
    , texCoord(texCoord)
{
}
VertexData3D::VertexData3D(float x, float y, float z, float u, float v)
    : position({ x, y, z })
    , texCoord({ u, v })
{
}
}