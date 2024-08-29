#include <Graphics/VertexTexCoord2D.hpp>

namespace Lib::Graphics {
VertexTexCoord2D::VertexTexCoord2D(const Math::Vector2& position, const Math::Vector2& texCoord)
    : position(position)
    , texCoord(texCoord)
{
}
VertexTexCoord2D::VertexTexCoord2D(float x, float y, float u, float v)
    : position({ x, y })
    , texCoord({ u, v })
{
}
}