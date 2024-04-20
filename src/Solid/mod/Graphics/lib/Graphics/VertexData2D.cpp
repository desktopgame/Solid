#include <Graphics/VertexData2D.hpp>

namespace Lib::Graphics {
VertexData2D::VertexData2D(const Math::Vector2& position, const Math::Vector2& texCoord)
    : position(position)
    , texCoord(texCoord)
{
}
}