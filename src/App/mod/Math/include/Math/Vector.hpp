#pragma once
#include <Math/Mathf.hpp>
#include <array>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Lib::Math {
template <typename T, int32_t N>
struct VectorT {
public:
    explicit VectorT()
        : components()
    {
    }

    explicit VectorT(std::array<T, N> src)
        : components(src)
    {
    }

    explicit VectorT(VectorT<T, N - 1> src, T a)
        : components()
    {
        std::copy(src.components.begin(), src.components.end(), components.begin());
        std::get<N - 1>(this->components) = a;
    }

    explicit VectorT(T a)
        : components()
    {
        reset(a);
    }

    // DotHelper

    template <int32_t NumV>
    struct DotHelper {
        using Type = T;

        static inline Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            Type sum = static_cast<Type>(0);
            for (int32_t i = 0; i < N; i++) {
                // sum += (a[i] * b[i]);
                sum = std::fmaf(a[i], b[i], sum);
            }
            return sum;
        }
    };

    // CrossHelper

    template <int32_t NumV>
    struct CrossHelper {
        using Type = void*&;

        static Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct CrossHelper<2> {
        static inline constexpr int32_t NumV = 2;
        using Type = T;

        static inline Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            return a.x() * b.y() - a.y() * b.x();
        }
    };

    template <>
    struct CrossHelper<3> {
        static inline constexpr int32_t NumV = 3;
        using Type = VectorT<T, NumV>;

        static inline Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            return Type(std::array<T, NumV> {
                a.y() * b.z() - b.y() * a.z(),
                a.z() * b.x() - b.z() * a.x(),
                a.x() * b.y() - b.x() * a.y() });
        }
    };

    // DistanceHelper

    template <int32_t NumV>
    struct DistanceHelper {
        static T compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            T sum = static_cast<T>(0);
            for (int32_t i = 0; i < N; i++) {
                T av = a.components.at(i);
                T bv = b.components.at(i);
                T diff = av - bv;
                sum += diff * diff;
            }
            return static_cast<T>(Mathf::sqrt(sum));
        }
    };

    inline T& at(int32_t index)
    {
        return components[index];
    }

    inline T at(int32_t index) const
    {
        return components[index];
    }

    inline T& x()
    {
        static_assert(N >= 1);
        return std::get<0>(components);
    }

    inline T x() const
    {

        static_assert(N >= 1);
        return std::get<0>(components);
    }

    inline T& y()
    {
        static_assert(N >= 2);
        return std::get<1>(components);
    }

    inline T y() const
    {
        static_assert(N >= 2);
        return std::get<1>(components);
    }

    inline T& z()
    {
        static_assert(N >= 3);
        return std::get<2>(components);
    }

    inline T z() const
    {
        static_assert(N >= 3);
        return std::get<2>(components);
    }

    inline T& w()
    {
        static_assert(N >= 4);
        return std::get<3>(components);
    }

    inline T w() const
    {
        static_assert(N >= 4);
        return std::get<3>(components);
    }

    inline T& r()
    {
        return x();
    }

    inline T r() const
    {
        return x();
    }

    inline T& g()
    {
        return y();
    }

    inline T g() const
    {
        return y();
    }

    inline T& b()
    {
        return z();
    }

    inline T b() const
    {
        return z();
    }

    inline T& a()
    {
        return w();
    }

    inline T a() const
    {
        return w();
    }

    inline const T& operator[](std::size_t i) const
    {
        return components[i];
    }

    inline T& operator[](std::size_t i)
    {
        return components[i];
    }

    std::string str() const
    {
        std::stringstream ss;
        ss << '(';
        for (int32_t i = 0; i < N; i++) {
            ss << components.at(i);
            if (i != N - 1) {
                ss << ',';
            }
        }
        ss << ')';
        return ss.str();
    }

    T* data()
    {
        return this->components.data();
    }

    const T* data() const
    {
        return this->components.data();
    }

    void reset(T a)
    {
        components.fill(a);
    }

    inline T length() const
    {
        T p = static_cast<T>(0);
        for (int32_t i = 0; i < N; i++) {
            p += static_cast<T>(std::pow(components[i], 2));
        }
        return static_cast<T>(std::sqrt(p));
    }

    void normalize()
    {
        T len = length();
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] / len;
        }
    }

    static VectorT<T, N> normalized(VectorT<T, N> v)
    {
        v.normalize();
        return v;
    }

    static inline typename DotHelper<N>::Type dot(const VectorT<T, N>& a, const VectorT<T, N>& b)
    {
        return DotHelper<N>::compute(a, b);
    }

    static inline typename CrossHelper<N>::Type cross(const VectorT<T, N>& a, const VectorT<T, N>& b)
    {
        return CrossHelper<N>::compute(a, b);
    }

    static T distance(const VectorT<T, N>& a, const VectorT<T, N>& b)
    {
        return DistanceHelper<N>::compute(a, b);
    }

    explicit operator VectorT<int32_t, N>() const
    {
        VectorT<int32_t, N> v;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            v.components[i] = static_cast<int32_t>(components[i]);
        }
        return v;
    }

    explicit operator VectorT<float, N>() const
    {
        VectorT<float, N> v;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            v.components[i] = static_cast<float>(components[i]);
        }
        return v;
    }

    explicit operator VectorT<T, N - 1>() const
    {
        std::array<T, N - 1> a;
        std::copy(components.begin(), components.begin() + (N - 1), a.begin());
        return VectorT<T, N - 1>(a);
    }

    inline VectorT<T, N>& operator+=(const VectorT<T, N>& a)
    {
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] + a[i];
        }
        return *this;
    }

    inline VectorT<T, N>& operator-=(const VectorT<T, N>& a)
    {
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] - a[i];
        }
        return *this;
    }

    inline VectorT<T, N>& operator*=(const VectorT<T, N>& a)
    {
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] * a[i];
        }
        return *this;
    }

    inline VectorT<T, N>& operator/=(const VectorT<T, N>& a)
    {
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] / a[i];
        }
        return *this;
    }

    inline VectorT<T, N>& operator*=(const T scale)
    {
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] * scale;
        }
        return *this;
    }

    inline VectorT<T, N>& operator/=(const T scale)
    {
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] / scale;
        }
        return *this;
    }

    inline VectorT<T, N> operator-() const
    {
        VectorT<T, N> v;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
        for (int32_t i = 0; i < N; i++) {
            v.components[i] = -components[i];
        }
        return v;
    }

    friend std::ostream& operator<<(std::ostream& os, const VectorT<T, N>& a)
    {
        os << a.str();
        return os;
    }

    std::array<T, N> components;
};

template <typename T, int32_t N>
inline VectorT<T, N> operator+(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] + b[i];
    }
    return ret;
}

template <typename T, int32_t N>
inline VectorT<T, N> operator-(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] - b[i];
    }
    return ret;
}

template <typename T, int32_t N>
inline VectorT<T, N> operator*(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] * b[i];
    }
    return ret;
}

template <typename T, int32_t N>
inline VectorT<T, N> operator/(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] / b[i];
    }
    return ret;
}

template <typename T, int32_t N>
inline VectorT<T, N> operator*(const VectorT<T, N>& a, T scale)
{
    VectorT<T, N> ret;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] * scale;
    }
    return ret;
}

template <typename T, int32_t N>
inline VectorT<T, N> operator/(const VectorT<T, N>& a, T scale)
{
    VectorT<T, N> ret;
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] / scale;
    }
    return ret;
}

template <typename T, int32_t N>
inline bool operator==(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
#ifdef _MSC_VER
#pragma loop(unroll)
#else
#pragma unroll
#endif
    for (int32_t i = 0; i < N; i++) {
        if (!Mathf::equals(a.components[i], b.components[i])) {
            return false;
        }
    }
    return true;
}

template <typename T, int32_t N>
inline bool operator!=(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    return !(a == b);
}

template <typename T>
VectorT<T, 2> makeVector2(T x, T y)
{
    VectorT<T, 2> v;
    v.x() = x;
    v.y() = y;
    return v;
}

template <typename T>
VectorT<T, 3> makeVector3(T x, T y, T z)
{
    VectorT<T, 3> v;
    v.x() = x;
    v.y() = y;
    v.z() = z;
    return v;
}

template <typename T>
VectorT<T, 4> makeVector4(T x, T y, T z, T w)
{
    VectorT<T, 4> v;
    v.x() = x;
    v.y() = y;
    v.z() = z;
    v.w() = w;
    return v;
}

using Vector2 = VectorT<float, 2>;
using Vector3 = VectorT<float, 3>;
using Vector4 = VectorT<float, 4>;
using IntVector2 = VectorT<int32_t, 2>;
using IntVector3 = VectorT<int32_t, 3>;
using IntVector4 = VectorT<int32_t, 4>;
}
