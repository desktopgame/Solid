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

        static Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            Type sum = static_cast<Type>(0);
            for (int32_t i = 0; i < N; i++) {
                sum += (a[i] * b[i]);
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

        static Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
        {
            return a.x() * b.y() - a.y() * b.x();
        }
    };

    template <>
    struct CrossHelper<3> {
        static inline constexpr int32_t NumV = 3;
        using Type = VectorT<T, NumV>;

        static Type compute(const VectorT<T, NumV>& a, const VectorT<T, NumV>& b)
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

    T& at(int32_t index)
    {
        return components.at(index);
    }

    T at(int32_t index) const
    {
        return components.at(index);
    }

    T& x()
    {
        static_assert(N >= 1);
        return components.at(0);
    }

    T x() const
    {

        static_assert(N >= 1);
        return components.at(0);
    }

    T& y()
    {
        static_assert(N >= 2);
        return components.at(1);
    }

    T y() const
    {
        static_assert(N >= 2);
        return components.at(1);
    }

    T& z()
    {
        static_assert(N >= 3);
        return components.at(2);
    }

    T z() const
    {
        static_assert(N >= 3);
        return components.at(2);
    }

    T& w()
    {
        static_assert(N >= 4);
        return components.at(3);
    }

    T w() const
    {
        static_assert(N >= 4);
        return components.at(3);
    }

    T& r()
    {
        return x();
    }

    T r() const
    {
        return x();
    }

    T& g()
    {
        return y();
    }

    T g() const
    {
        return y();
    }

    T& b()
    {
        return z();
    }

    T b() const
    {
        return z();
    }

    T& a()
    {
        return w();
    }

    T a() const
    {
        return w();
    }

    const T& operator[](std::size_t i) const
    {
        return components[i];
    }

    T& operator[](std::size_t i)
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

    T length() const
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

    static typename DotHelper<N>::Type dot(const VectorT<T, N>& a, const VectorT<T, N>& b)
    {
        return DotHelper<N>::compute(a, b);
    }

    static typename CrossHelper<N>::Type cross(const VectorT<T, N>& a, const VectorT<T, N>& b)
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
        for (int32_t i = 0; i < N; i++) {
            v.components[i] = static_cast<int32_t>(components[i]);
        }
        return v;
    }

    explicit operator VectorT<float, N>() const
    {
        VectorT<float, N> v;
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

    VectorT<T, N>& operator+=(const VectorT<T, N>& a)
    {
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] + a[i];
        }
        return *this;
    }

    VectorT<T, N>& operator-=(const VectorT<T, N>& a)
    {
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] - a[i];
        }
        return *this;
    }

    VectorT<T, N>& operator*=(const VectorT<T, N>& a)
    {
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] * a[i];
        }
        return *this;
    }

    VectorT<T, N>& operator/=(const VectorT<T, N>& a)
    {
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] / a[i];
        }
        return *this;
    }

    VectorT<T, N>& operator*=(const T scale)
    {
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] * scale;
        }
        return *this;
    }

    VectorT<T, N>& operator/=(const T scale)
    {
        for (int32_t i = 0; i < N; i++) {
            components[i] = components[i] / scale;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const VectorT<T, N>& a)
    {
        os << a.str();
        return os;
    }

    std::array<T, N> components;
};

template <typename T, int32_t N>
VectorT<T, N> operator+(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] + b[i];
    }
    return ret;
}

template <typename T, int32_t N>
VectorT<T, N> operator-(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] - b[i];
    }
    return ret;
}

template <typename T, int32_t N>
VectorT<T, N> operator*(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] * b[i];
    }
    return ret;
}

template <typename T, int32_t N>
VectorT<T, N> operator/(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    VectorT<T, N> ret;
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] / b[i];
    }
    return ret;
}

template <typename T, int32_t N>
VectorT<T, N> operator*(const VectorT<T, N>& a, T scale)
{
    VectorT<T, N> ret;
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] * scale;
    }
    return ret;
}

template <typename T, int32_t N>
VectorT<T, N> operator/(const VectorT<T, N>& a, T scale)
{
    VectorT<T, N> ret;
    for (int32_t i = 0; i < N; i++) {
        ret[i] = a[i] / scale;
    }
    return ret;
}

template <typename T, int32_t N>
bool operator==(const VectorT<T, N>& a, const VectorT<T, N>& b)
{
    for (int32_t i = 0; i < N; i++) {
        if (!Mathf::equals(a.components[i], b.components[i])) {
            return false;
        }
    }
    return true;
}

template <typename T, int32_t N>
bool operator!=(const VectorT<T, N>& a, const VectorT<T, N>& b)
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
