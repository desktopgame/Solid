#pragma once
#include <Math/Vector.hpp>

namespace Lib::Math::Geom {
template <typename T>
class PlaneT {
public:
    VectorT<T, 3> center;
    VectorT<T, 3> normal;
};
using Plane = PlaneT<float>;
}