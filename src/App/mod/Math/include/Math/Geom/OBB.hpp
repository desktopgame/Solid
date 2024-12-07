#pragma once
#include <Math/Vector.hpp>

namespace Lib::Math::Geom {
class OBB {
public:
    Vector3 center;
    Vector3 size;
    std::array<Vector3, 3> axes;
};
}