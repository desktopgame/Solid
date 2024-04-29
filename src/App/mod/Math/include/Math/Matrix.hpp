#pragma once
#include <Math/Mathf.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Lib::Math {
template <typename T>
struct MatrixT {
    inline static constexpr int32_t RowNum = 4;
    inline static constexpr int32_t ColumnNum = 4;

    explicit MatrixT()
        : components()
    {
        for (int32_t i = 0; i < RowNum; i++) {
            components[(i * ColumnNum) + i] = 1;
        }
    }

    explicit MatrixT(const std::array<VectorT<T, ColumnNum>, RowNum>& table)
    {
        for (int32_t i = 0; i < RowNum; i++) {
            const VectorT<T, ColumnNum>& line = table.at(i);
            for (int32_t j = 0; j < ColumnNum; j++) {
                this->at(i, j) = line.components.at(j);
            }
        }
    }

    // mutable

    std::array<std::reference_wrapper<T>, ColumnNum> row(int32_t index)
    {
        T& e0 = components[(index * ColumnNum) + 0];
        T& e1 = components[(index * ColumnNum) + 1];
        T& e2 = components[(index * ColumnNum) + 2];
        T& e3 = components[(index * ColumnNum) + 3];

        std::array<std::reference_wrapper<T>, 4> a {
            e0,
            e1,
            e2,
            e3
        };
        return a;
    }

    std::array<std::reference_wrapper<T>, RowNum> column(int32_t index)
    {
        T& e0 = components[index];
        T& e1 = components[(1 * ColumnNum) + index + 0];
        T& e2 = components[(2 * ColumnNum) + index + 0];
        T& e3 = components[(3 * ColumnNum) + index + 0];

        std::array<std::reference_wrapper<T>, 4> a {
            e0,
            e1,
            e2,
            e3
        };
        return a;
    }

    T& at(int32_t row, int32_t col)
    {
        return components[(row * ColumnNum) + col];
    }

    // immutable

    std::array<std::reference_wrapper<const T>, ColumnNum> row(int32_t index) const
    {
        const T& e0 = components[(index * ColumnNum) + 0];
        const T& e1 = components[(index * ColumnNum) + 1];
        const T& e2 = components[(index * ColumnNum) + 2];
        const T& e3 = components[(index * ColumnNum) + 3];

        std::array<std::reference_wrapper<const T>, 4> a {
            e0,
            e1,
            e2,
            e3
        };
        return a;
    }

    std::array<std::reference_wrapper<const T>, RowNum> column(int32_t index) const
    {
        const T& e0 = components[index];
        const T& e1 = components[(1 * ColumnNum) + index + 0];
        const T& e2 = components[(2 * ColumnNum) + index + 0];
        const T& e3 = components[(3 * ColumnNum) + index + 0];

        std::array<std::reference_wrapper<const T>, 4> a {
            e0,
            e1,
            e2,
            e3
        };
        return a;
    }

    T at(int32_t row, int32_t col) const
    {
        return components[(row * ColumnNum) + col];
    }

    // operator

    const std::array<std::reference_wrapper<const T>, ColumnNum> operator[](std::size_t i) const
    {
        return row(static_cast<int32_t>(i));
    }
    std::array<std::reference_wrapper<T>, ColumnNum> operator[](std::size_t i)
    {
        return row(static_cast<int32_t>(i));
    }

    // utility

    static MatrixT<T> multiply(const MatrixT<T>& a, const MatrixT<T>& b)
    {
        MatrixT<T> m;
        for (int32_t i = 0; i < RowNum; i++) {
            for (int32_t j = 0; j < ColumnNum; j++) {
                std::array<std::reference_wrapper<const T>, ColumnNum> row = a.row(i);
                std::array<std::reference_wrapper<const T>, RowNum> col = b.column(j);
                T sum = static_cast<T>(0);
                for (int32_t k = 0; k < RowNum; k++) {
                    sum += row[k] * col[k];
                }
                m.at(i, j) = sum;
            }
        }
        return m;
    }

    static VectorT<T, 2> multiply(const MatrixT<T>& a, const VectorT<T, 2>& b)
    {
        std::array<std::reference_wrapper<const T>, ColumnNum> r1 = a.column(0);
        std::array<std::reference_wrapper<const T>, ColumnNum> r2 = a.column(1);
        return VectorT<T, 2>({ (r1.at(0) * b.at(0)) + (r1.at(1) * b.at(1)) + r1.at(2) + r1.at(3),
            (r2.at(0) * b.at(0)) + (r2.at(1) * b.at(1)) + r2.at(2) + r2.at(3) });
    }

    static VectorT<T, 3> multiply(const MatrixT<T>& a, const VectorT<T, 3>& b)
    {
        std::array<std::reference_wrapper<const T>, ColumnNum> r1 = a.column(0);
        std::array<std::reference_wrapper<const T>, ColumnNum> r2 = a.column(1);
        std::array<std::reference_wrapper<const T>, ColumnNum> r3 = a.column(2);
        return VectorT<T, 3>({ (r1.at(0) * b.at(0)) + (r1.at(1) * b.at(1)) + (r1.at(2) * b.at(2)) + r1.at(3),
            (r2.at(0) * b.at(0)) + (r2.at(1) * b.at(1)) + (r2.at(2) * b.at(2) + r2.at(3)),
            (r3.at(0) * b.at(0)) + (r3.at(1) * b.at(1)) + (r3.at(2) * b.at(2)) + r3.at(3) });
    }

    static MatrixT<T> translate(VectorT<T, 3> v)
    {
        MatrixT<T> m;
        m.at(3, 0) = v.x();
        m.at(3, 1) = v.y();
        m.at(3, 2) = v.z();
        return m;
    }

    static MatrixT<T> rotateX(T angle)
    {
        MatrixT<T> m;
        m.at(1, 1) = Mathf::cos(angle);
        m.at(1, 2) = -Mathf::sin(angle);
        m.at(2, 1) = Mathf::sin(angle);
        m.at(2, 2) = Mathf::cos(angle);
        return m;
    }

    static MatrixT<T> rotateY(T angle)
    {
        MatrixT<T> m;
        m.at(0, 0) = Mathf::cos(angle);
        m.at(0, 2) = Mathf::sin(angle);
        m.at(2, 0) = -Mathf::sin(angle);
        m.at(2, 2) = Mathf::cos(angle);
        return m;
    }

    static MatrixT<T> rotateZ(T angle)
    {
        MatrixT<T> m;
        m.at(0, 0) = Mathf::cos(angle);
        m.at(0, 1) = -Mathf::sin(angle);
        m.at(1, 0) = Mathf::sin(angle);
        m.at(1, 1) = Mathf::cos(angle);
        return m;
    }

    static MatrixT<T> scale(VectorT<T, 3> v)
    {
        MatrixT<T> m;
        m.at(0, 0) = v.x();
        m.at(1, 1) = v.y();
        m.at(2, 2) = v.z();
        return m;
    }

    static MatrixT<T> inverse(const MatrixT<T>& m)
    {
        // see: https://risalc.info/src/inverse-cofactor-ex4.html
        MatrixT<T> ret;
        T a11 = m.at(0, 0);
        T a12 = m.at(0, 1);
        T a13 = m.at(0, 2);
        T a14 = m.at(0, 3);
        T a21 = m.at(1, 0);
        T a22 = m.at(1, 1);
        T a23 = m.at(1, 2);
        T a24 = m.at(1, 3);
        T a31 = m.at(2, 0);
        T a32 = m.at(2, 1);
        T a33 = m.at(2, 2);
        T a34 = m.at(2, 3);
        T a41 = m.at(3, 0);
        T a42 = m.at(3, 1);
        T a43 = m.at(3, 2);
        T a44 = m.at(3, 3);
        T scale = static_cast<T>(1) / ((a11 * a22 * a33 * a44) + (a11 * a23 * a34 * a42) + (a11 * a24 * a32 * a43) -

                      (a11 * a24 * a33 * a42) - (a11 * a23 * a32 * a44) - (a11 * a22 * a34 * a43) -

                      (a12 * a21 * a33 * a44) - (a13 * a21 * a34 * a42) - (a14 * a21 * a32 * a43) +

                      (a14 * a21 * a33 * a42) + (a13 * a21 * a32 * a44) + (a12 * a21 * a34 * a43) +

                      (a12 * a23 * a31 * a44) + (a13 * a24 * a31 * a42) + (a14 * a22 * a31 * a43) -

                      (a14 * a23 * a31 * a42) - (a13 * a22 * a31 * a44) - (a12 * a24 * a31 * a43) -

                      (a12 * a23 * a34 * a41) - (a13 * a24 * a32 * a41) - (a14 * a22 * a33 * a41) +

                      (a14 * a23 * a32 * a41) + (a13 * a22 * a34 * a41) + (a12 * a24 * a33 * a41));
        ret.at(0, 0) = scale * ((a22 * a33 * a44) + (a23 * a34 * a42) + (a24 * a32 * a43) - (a24 * a33 * a42) - (a23 * a32 * a44) - (a22 * a34 * a43));
        ret.at(0, 1) = scale * ((-a12 * a33 * a44) - (a13 * a34 * a42) - (a14 * a32 * a43) + (a14 * a33 * a42) + (a13 * a32 * a44) + (a12 * a34 * a43));
        ret.at(0, 2) = scale * ((a12 * a23 * a44) + (a13 * a24 * a42) + (a14 * a22 * a43) - (a14 * a23 * a42) - (a13 * a22 * a44) - (a12 * a24 * a43));
        ret.at(0, 3) = scale * ((-a12 * a23 * a34) - (a13 * a24 * a32) - (a14 * a22 * a33) + (a14 * a23 * a32) + (a13 * a22 * a34) + (a12 * a24 * a33));

        ret.at(1, 0) = scale * ((-a21 * a33 * a44) - (a23 * a34 * a41) - (a24 * a31 * a43) + (a24 * a33 * a41) + (a23 * a31 * a44) + (a21 * a34 * a43));
        ret.at(1, 1) = scale * ((a11 * a33 * a44) + (a13 * a34 * a41) + (a14 * a31 * a43) - (a14 * a33 * a41) - (a13 * a31 * a44) - (a11 * a34 * a43));
        ret.at(1, 2) = scale * ((-a11 * a23 * a44) - (a13 * a24 * a41) - (a14 * a21 * a43) + (a14 * a23 * a41) + (a13 * a21 * a44) + (a11 * a24 * a43));
        ret.at(1, 3) = scale * ((a11 * a23 * a34) + (a13 * a24 * a31) + (a14 * a21 * a33) - (a14 * a23 * a31) - (a13 * a21 * a34) - (a11 * a24 * a33));

        ret.at(2, 0) = scale * ((a21 * a32 * a44) + (a22 * a34 * a41) + (a24 * a31 * a42) - (a24 * a32 * a41) - (a22 * a31 * a44) - (a21 * a34 * a42));
        ret.at(2, 1) = scale * ((-a11 * a32 * a44) - (a12 * a34 * a41) - (a14 * a31 * a42) + (a14 * a32 * a41) + (a12 * a31 * a44) + (a11 * a34 * a42));
        ret.at(2, 2) = scale * ((a11 * a22 * a44) + (a12 * a24 * a41) + (a14 * a21 * a42) - (a14 * a22 * a41) - (a12 * a21 * a44) - (a11 * a24 * a42));
        ret.at(2, 3) = scale * ((-a11 * a22 * a34) - (a12 * a24 * a31) - (a14 * a21 * a32) + (a14 * a22 * a31) + (a12 * a21 * a34) + (a11 * a24 * a32));

        ret.at(3, 0) = scale * ((-a21 * a32 * a43) - (a22 * a33 * a41) - (a23 * a31 * a42) + (a23 * a32 * a41) + (a22 * a31 * a43) + (a21 * a33 * a42));
        ret.at(3, 1) = scale * ((a11 * a32 * a43) + (a12 * a33 * a41) + (a13 * a31 * a42) - (a13 * a32 * a41) - (a12 * a31 * a43) - (a11 * a33 * a42));
        ret.at(3, 2) = scale * ((-a11 * a22 * a43) - (a12 * a23 * a41) - (a13 * a21 * a42) + (a13 * a22 * a41) + (a12 * a21 * a43) + (a11 * a23 * a42));
        ret.at(3, 3) = scale * ((a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32) - (a13 * a22 * a31) - (a12 * a21 * a33) - (a11 * a23 * a32));
        return ret;
    }

    static MatrixT<T> ortho(T width, T height, T zNear, T zFar)
    {
        MatrixT<T> ret;
        T two = static_cast<T>(2);
        T zero = static_cast<T>(0);
        // T one = static_cast<T>(1);
        // x, y -> 0~2 -> -1~+1
        // z -> 0~2 -> -1~+1
        ret.at(0, 0) = two / (width);
        ret.at(1, 1) = two / (height);
        ret.at(2, 2) = static_cast<T>(1) / (zFar - zNear);
        ret.at(3, 0) = zero;
        ret.at(3, 1) = zero;
        ret.at(3, 2) = -zNear / (zFar - zNear);
        return ret;
    }

    static MatrixT<T> perspective(T fovy, T aspectRatio, T zNear, T zFar)
    {
        // see: https://learn.microsoft.com/ja-jp/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
        MatrixT<T> ret;
        T one = static_cast<T>(1);
        T zero = static_cast<T>(0);
        T yScale = one / std::tan(fovy / static_cast<T>(2));
        T xScale = yScale / aspectRatio;
        std::fill(ret.components.begin(), ret.components.end(), zero);
        ret.at(0, 0) = xScale;
        ret.at(1, 1) = yScale;
        ret.at(2, 2) = zFar / (zFar - zNear);
        ret.at(2, 3) = (one);
        ret.at(3, 2) = -zNear * zFar / (zFar - zNear);
        return ret;
    }

    static MatrixT<T> lookAt(const VectorT<T, 3>& eye, const VectorT<T, 3>& at, const VectorT<T, 3>& up)
    {
        VectorT<T, 3> f = VectorT<T, 3>::normalized(at - eye);
        VectorT<T, 3> s = VectorT<T, 3>::normalized(VectorT<T, 3>::cross(up, f));
        VectorT<T, 3> u = VectorT<T, 3>::cross(f, s);
        MatrixT<T> ret;
        ret.at(0, 0) = s.x();
        ret.at(1, 0) = s.y();
        ret.at(2, 0) = s.z();
        ret.at(0, 1) = u.x();
        ret.at(1, 1) = u.y();
        ret.at(2, 1) = u.z();
        ret.at(0, 2) = f.x();
        ret.at(1, 2) = f.y();
        ret.at(2, 2) = f.z();
        ret.at(3, 0) = -VectorT<T, 3>::dot(s, eye);
        ret.at(3, 1) = -VectorT<T, 3>::dot(u, eye);
        ret.at(3, 2) = -VectorT<T, 3>::dot(f, eye);
        return ret;
    }

    static MatrixT<T> transpose(const MatrixT<T>& m)
    {
        MatrixT<T> ret;
        ret.at(0, 0) = m.at(0, 0);
        ret.at(0, 1) = m.at(1, 0);
        ret.at(0, 2) = m.at(2, 0);
        ret.at(0, 3) = m.at(3, 0);

        ret.at(1, 0) = m.at(0, 1);
        ret.at(1, 1) = m.at(1, 1);
        ret.at(1, 2) = m.at(2, 1);
        ret.at(1, 3) = m.at(3, 1);

        ret.at(2, 0) = m.at(0, 2);
        ret.at(2, 1) = m.at(1, 2);
        ret.at(2, 2) = m.at(2, 2);
        ret.at(2, 3) = m.at(3, 2);

        ret.at(3, 0) = m.at(0, 3);
        ret.at(3, 1) = m.at(1, 3);
        ret.at(3, 2) = m.at(2, 3);
        ret.at(3, 3) = m.at(3, 3);
        return ret;
    }

    std::string str() const
    {
        std::stringstream ss;
        ss << std::endl;
        for (int32_t i = 0; i < RowNum; i++) {
            int32_t start = i * ColumnNum;
            int32_t end = start + ColumnNum;
            ss << "[";
            for (int32_t n = start; n < end; n++) {
                if (n == start) {
                    ss << " ";
                }
                char block[16] = { 0 };
                float num = static_cast<float>(components[n]);
                ::sprintf(block, "%.2f", num);
                ss << block;
                if (n != end - 1) {
                    ss << ", ";
                } else {
                    ss << " ";
                }
            }
            ss << "]" << std::endl;
        }
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

    friend std::ostream& operator<<(std::ostream& os, const MatrixT<T>& m)
    {
        os << m.str();
        return os;
    }

    std::array<T, RowNum * ColumnNum> components;
};
template <typename T>
bool operator==(const MatrixT<T>& a, const MatrixT<T>& b)
{
    for (int32_t i = 0; i < MatrixT<T>::RowNum * MatrixT<T>::ColumnNum; i++) {
        if (!Mathf::equals(a.components[i], b.components[i])) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool operator!=(const MatrixT<T>& a, const MatrixT<T>& b)
{
    return !(a == b);
}

template <typename T>
MatrixT<T> operator*(const MatrixT<T>& a, const MatrixT<T>& b)
{
    return MatrixT<T>::multiply(a, b);
}

using Matrix = MatrixT<float>;
}
