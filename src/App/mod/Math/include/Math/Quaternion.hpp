#pragma once
#include <Math/Mathf.hpp>
#include <Math/Vector.hpp>
#include <string>

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

    std::string str() const
    {
        std::stringstream ss;
        ss << '(' << x << ',' << y << ',' << z << ',' << w << ')';
        return ss.str();
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

    static QuaternionT<T> angleAxis(float degree, VectorT<T, 3> axis)
    {
        // see: http://marupeke296.com/DXG_No10_Quaternion.html
        QuaternionT<T> q = QuaternionT<T>();
        float sin = Mathf::sin((degree / 2.0f) * Mathf::Deg2Rad);
        float cos = Mathf::cos((degree / 2.0f) * Mathf::Deg2Rad);
        q.x = axis.x() * sin;
        q.y = axis.y() * sin;
        q.z = axis.z() * sin;
        q.w = cos;
        return q;
    }

    static QuaternionT<T> multiply(QuaternionT<T> q1, QuaternionT<T> q2)
    {
        // see: http://marupeke296.com/DXG_No10_Quaternion.html
        VectorT<T, 3> v1 = VectorT<T, 3>({ q1.x, q1.y, q1.z });
        VectorT<T, 3> v2 = VectorT<T, 3>({ q2.x, q2.y, q2.z });
        VectorT<T, 3> qv = (v2 * q1.w) + (v1 * q2.w) + VectorT<T, 3>::cross(v1, v2);
        QuaternionT<T> q3 = QuaternionT<T>();
        q3.x = qv.x();
        q3.y = qv.y();
        q3.z = qv.z();
        q3.w = (q1.w * q2.w) - (VectorT<T, 3>::dot(v1, v2));
        return q3;
    }

    static VectorT<T, 3> transform(QuaternionT<T> q, VectorT<T, 3> v)
    {
        QuaternionT<T> vq = QuaternionT<T>();
        vq.x = v.x();
        vq.y = v.y();
        vq.z = v.z();
        vq.w = static_cast<T>(0);

        QuaternionT<T> q2 = QuaternionT<T>::conjugate(q);
        QuaternionT<T> qr = QuaternionT<T>::multiply(QuaternionT<T>::multiply(q, vq), q2);
        return VectorT<T, 3>({ qr.x, qr.y, qr.z });
    }

    friend std::ostream& operator<<(std::ostream& os, const QuaternionT<T>& a)
    {
        os << a.str();
        return os;
    }

    T x;
    T y;
    T z;
    T w;
};

template <typename T>
bool operator==(const QuaternionT<T> a, const QuaternionT<T>& b)
{
    bool eqX = Mathf::equals(a.x, b.x);
    bool eqY = Mathf::equals(a.y, b.y);
    bool eqZ = Mathf::equals(a.z, b.z);
    return eqX && eqY && eqZ;
}

template <typename T>
QuaternionT<T> operator*(const QuaternionT<T>& a, const QuaternionT<T>& b)
{
    return QuaternionT<T>::multiply(a, b);
}
using Quaternion = QuaternionT<float>;
}