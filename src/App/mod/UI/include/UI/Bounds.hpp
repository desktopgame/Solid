#pragma once

namespace Lib::UI {
template <typename T>
class BoundsT {
public:
    T x;
    T y;
    T width;
    T height;
};

using Bounds = BoundsT<float>;
}