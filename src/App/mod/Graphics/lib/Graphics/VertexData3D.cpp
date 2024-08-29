#include <Graphics/VertexData3D.hpp>

namespace Lib::Graphics {
VertexTexCoord3D::VertexTexCoord3D(const Math::Vector3& position, const Math::Vector2& texCoord)
    : position(position)
    , texCoord(texCoord)
{
}
VertexTexCoord3D::VertexTexCoord3D(float x, float y, float z, float u, float v)
    : position({ x, y, z })
    , texCoord({ u, v })
{
}
}