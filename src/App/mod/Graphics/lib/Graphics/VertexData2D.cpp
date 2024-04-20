#include <Graphics/VertexData2D.hpp>

namespace Lib::Graphics {
VertexData2D::VertexData2D(const Math::Vector2& position, const Math::Vector2& texCoord)
    : position(position)
    , texCoord(texCoord)
{
}
VertexData2D::VertexData2D(float x, float y, float u, float v)
    : position({ x, y })
    , texCoord({ u, v })
{
}
}