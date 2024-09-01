#include <Graphics/FontMetrics.hpp>
namespace Lib::Graphics {

FontMetrics::FontMetrics(Math::IntVector2 size, Math::IntVector2 bearing,
    Math::IntVector2 advance)
    : size(size)
    , bearing(bearing)
    , advance(advance)
{
}
}
