#pragma once
#include <Math/Vector.hpp>

namespace Lib::Math::Geom {
template <typename T>
class OBBT {
public:
    VectorT<T, 3> center;
    VectorT<T, 3> size;
    std::array<VectorT<T, 3>, 3> axes;
};
using OBB = OBBT<float>;
}