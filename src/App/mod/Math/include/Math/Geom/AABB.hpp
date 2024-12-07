#pragma once
#include <Math/Vector.hpp>

namespace Lib::Math::Geom {
template <typename T>
class AABBT {
public:
    VectorT<T, 3> min;
    VectorT<T, 3> max;
};
using AABB = AABBT<float>;
}