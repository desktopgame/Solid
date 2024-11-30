#pragma once
#include <cmath>
#include <limits>

namespace Lib::Math {
template <typename T>
class Segment {
public:
    explicit Segment(T a, T b)
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

    Segment<T> logicAnd(const Segment<T>& other) const
    {
        if (contains(other.min, true) && contains(other.max, true)) {
            return other;
        } else if (!contains(other.min, true) && contains(other.max, true)) {
            return Segment<T>(min, other.max);
        } else if (contains(other.min, true) && !contains(other.max, true)) {
            return Segment<T>(other.min, max);
        } else {
            return Segment<T>(0, 0);
        }
    }

    Segment<T> logicOr(const Segment<T>& other) const
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
        return Segment<T>(newMin, newMax);
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
}