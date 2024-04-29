#pragma once
#include <Math/Mathf.hpp>

namespace Lib::Math {
template <typename T>
class QuaternionT {
public:
    explicit QuaternionT()
        : x(0)
        , y(0)
        , z(0)
        , w(static_cast<T>(1))
    {
    }

    T length()
    {
        T x2 = x * x;
        T y2 = y * y;
        T z2 = z * z;
        T w2 = w * w;
        return Mathf::sqrt(x2 + y2 + z2 + w2);
    }

    void normalize()
    {
        T len = length();
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }

    static QuaternionT<T> normalized(QuaternionT<T> q)
    {
        q.normalize();
        return q;
    }

    static QuaternionT<T> conjugate(QuaternionT<T> q)
    {
        q.x *= static_cast<T>(-1);
        q.y *= static_cast<T>(-1);
        q.z *= static_cast<T>(-1);
        return q;
    }

    static QuaternionT<T> inverse(QuaternionT<T> q)
    {
        QuaternionT<T> c = conjugate(q);
        T len = static_cast<T>(q.length());
        c.x /= len;
        c.y /= len;
        c.z /= len;
        return c;
    }

    T x;
    T y;
    T z;
    T w;
};
using Quaternion = QuaternionT<float>;
}