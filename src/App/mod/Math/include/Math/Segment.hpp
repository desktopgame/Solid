#pragma once
#include <cmath>
#include <limits>

namespace Lib::Math {
template <typename T>
class SegmentT {
public:
    explicit SegmentT(T a, T b)
    {
        if (a > b) {
            max = a;
            min = b;
        } else {
            max = b;
            min = a;
        }
    }

    bool contains(T value, bool inclusiveMax = false) const
    {
        if (inclusiveMax) {
            return value >= min && value <= max;
        }
        return value >= min && value < max;
    }

    SegmentT<T> logicAnd(const SegmentT<T>& other) const
    {
        if (contains(other.min, true) && contains(other.max, true)) {
            return other;
        } else if (!contains(other.min, true) && contains(other.max, true)) {
            return SegmentT<T>(min, other.max);
        } else if (contains(other.min, true) && !contains(other.max, true)) {
            return SegmentT<T>(other.min, max);
        } else {
            return SegmentT<T>(0, 0);
        }
    }

    SegmentT<T> logicOr(const SegmentT<T>& other) const
    {
        T newMin = static_cast<T>(0);
        if (min < other.min) {
            newMin = min;
        } else {
            newMin = other.min;
        }

        T newMax = static_cast<T>(0);
        if (max > other.max) {
            newMax = max;
        } else {
            newMax = other.max;
        }
        return SegmentT<T>(newMin, newMax);
    }

    float diff() const { return max - min; }

    float distance() const
    {
        float d = diff();
        if (d < 0.0f) {
            d = -d;
        }
        return d;
    }

    bool isZero() const
    {
        return distance() <= std::numeric_limits<float>::epsilon();
    }

    T min;
    T max;
};

using Segment = SegmentT<float>;
}