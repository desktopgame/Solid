#pragma once
#undef min
#undef max
#include <cmath>
#include <limits>

// clangでは特殊化された関数にstaticをつけてはいけないが、VC++ではつけなければいけない
#ifdef __clang__
#define INLINE_SPEC_STATIC
#else
#define INLINE_SPEC_STATIC static
#endif

namespace Lib::Math {

class Mathf {
public:
    inline static constexpr float Deg2Rad = 3.14f / 180.0f;
    inline static constexpr float Rad2Deg = 180.0f / 3.14f;

    template <typename T>
    inline static T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    inline static T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    inline static T map(T min, T max, T t)
    {
        return min + ((max - min) * t);
    }

    template <typename T>
    inline static T clamp(T min, T max, T current)
    {
        if (current < min) {
            current = min;
        }
        if (current > max) {
            current = max;
        }
        return current;
    }

    template <typename T>
    inline static T sin(T v)
    {
        return static_cast<T>(std::sin(static_cast<float>(v)));
    }

    template <typename T>
    inline static T cos(T v)
    {
        return static_cast<T>(std::cos(static_cast<float>(v)));
    }

    template <typename T>
    inline static T tan(T v)
    {
        return static_cast<T>(std::tan(static_cast<float>(v)));
    }

    template <typename T>
    inline static T asin(T v)
    {
        return static_cast<T>(std::asin(static_cast<float>(v)));
    }

    template <typename T>
    inline static T acos(T v)
    {
        return static_cast<T>(std::acos(static_cast<float>(v)));
    }

    template <typename T>
    inline static T atan(T v)
    {
        return static_cast<T>(std::atan(static_cast<float>(v)));
    }

    template <typename T>
    inline static T sqrt(T v)
    {
        return static_cast<T>(std::sqrt(static_cast<float>(v)));
    }

    template <typename T>
    inline static bool equals(T a, T b)
    {
        return a == b;
    }

    template <>
    INLINE_SPEC_STATIC bool equals<float>(float a, float b)
    {
        float diff = std::abs(a - b);
        return diff <= std::numeric_limits<float>::epsilon();
    }

    template <typename T>
    inline static bool in(T a, T b, T at)
    {
        return min(a, b) <= at && at < max(a, b);
    }

    template <typename T>
    inline static T sign(T a)
    {
        if (equals(a, 0.0f)) {
            return 0.0f;
        }
        return a > 0 ? static_cast<T>(1) : static_cast<T>(-1);
    }

private:
    Mathf() = delete;
    ~Mathf() = delete;
};
}

#undef INLINE_SPEC_STATIC