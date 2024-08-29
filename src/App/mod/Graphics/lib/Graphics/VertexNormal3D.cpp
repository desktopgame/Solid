#include <Graphics/VertexNormal3D.hpp>

namespace Lib::Graphics {
VertexNormal3D::VertexNormal3D(const Math::Vector3& position, const Math::Vector3& normal)
    : position(position)
    , normal(normal)
{
}
}