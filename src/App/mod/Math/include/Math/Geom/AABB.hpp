#pragma once
#include <Math/Vector.hpp>

namespace Lib::Math::Geom {
class AABB {
public:
    Vector3 min;
    Vector3 max;
};
}