#pragma once
#include <Math/Vector.hpp>
#include <array>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>

#ifndef MATH_MATRIX_MODE
#define MATH_MATRIX_MODE (1)
#endif

#define MATH_MATRIX_ROW_MAJOR (0)
#define MATH_MATRIX_COLUMN_MAJOR (1)
#define MATH_MATRIX_LH (0)
#define MATH_MATRIX_RH (1)

#if MATH_MATRIX_MODE == MATH_MATRIX_COLUMN_MAJOR
#define MATH_MATRIX_HAND MATH_MATRIX_RH
#else
#define MATH_MATRIX_HAND MATH_MATRIX_LH
#endif

namespace Lib::Math {
template <typename T, int RowNum, int ColumnNum>
struct MatrixT {
    explicit MatrixT()
        : components()
    {
        for (int i = 0; i < RowNum; i++) {
            components[(i * ColumnNum) + i] = 1;
        }
    }

    explicit MatrixT(const std::array<VectorT<T, ColumnNum>, RowNum>& table)
    {
        for (int i = 0; i < RowNum; i++) {
            const VectorT<T, ColumnNum>& line = table.at(i);
            for (int j = 0; j < ColumnNum; j++) {
                this->at(i, j) = line.components.at(j);
            }
        }
    }

    // RowHelper

    template <int RowNumV, int ColumnNumV>
    struct RowHelper {
        // static inline constexpr int Num = 0;
        static std::array<std::reference_wrapper<T>, ColumnNumV> get(std::array<T, RowNumV * ColumnNumV>& a, int index)
        {
            throw std::logic_error("not implemented");
        }
        static std::array<std::reference_wrapper<const T>, ColumnNumV> get(const std::array<T, RowNumV * ColumnNumV>& a, int index)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <int RowNumV>
    struct RowHelper<RowNumV, 2> {
        static inline constexpr int ColumnNumV = 2;
        static std::array<std::reference_wrapper<T>, ColumnNumV> get(std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            T& e0 = components[(index * ColumnNumV) + 0];
            T& e1 = components[(index * ColumnNumV) + 1];
            // T& e2 = components[(index * ColumnNumV) + 2];
            // T& e3 = components[(index * ColumnNumV) + 3];

            std::array<std::reference_wrapper<T>, ColumnNumV> a {
                e0,
                e1,
                // e2,
                // e3
            };
            return a;
        }
        static std::array<std::reference_wrapper<const T>, ColumnNumV> get(const std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            const T& e0 = components[(index * ColumnNumV) + 0];
            const T& e1 = components[(index * ColumnNumV) + 1];
            // const T& e2 = components[(index * ColumnNumV) + 2];
            // const T& e3 = components[(index * ColumnNumV) + 3];

            std::array<std::reference_wrapper<const T>, ColumnNumV> a {
                e0,
                e1,
                // e2,
                // e3
            };
            return a;
        }
    };

    template <int RowNumV>
    struct RowHelper<RowNumV, 3> {
        static inline constexpr int ColumnNumV = 3;
        static std::array<std::reference_wrapper<T>, ColumnNumV> get(std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            T& e0 = components[(index * ColumnNumV) + 0];
            T& e1 = components[(index * ColumnNumV) + 1];
            T& e2 = components[(index * ColumnNumV) + 2];
            // T& e3 = components[(index * ColumnNumV) + 3];

            std::array<std::reference_wrapper<T>, ColumnNumV> a {
                e0,
                e1,
                e2,
                // e3
            };
            return a;
        }
        static std::array<std::reference_wrapper<const T>, ColumnNumV> get(const std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            const T& e0 = components[(index * ColumnNumV) + 0];
            const T& e1 = components[(index * ColumnNumV) + 1];
            const T& e2 = components[(index * ColumnNumV) + 2];
            // const T& e3 = components[(index * ColumnNumV) + 3];

            std::array<std::reference_wrapper<const T>, ColumnNumV> a {
                e0,
                e1,
                e2,
                // e3
            };
            return a;
        }
    };

    template <int RowNumV>
    struct RowHelper<RowNumV, 4> {
        static inline constexpr int ColumnNumV = 4;
        static std::array<std::reference_wrapper<T>, ColumnNumV> get(std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            T& e0 = components[(index * ColumnNumV) + 0];
            T& e1 = components[(index * ColumnNumV) + 1];
            T& e2 = components[(index * ColumnNumV) + 2];
            T& e3 = components[(index * ColumnNumV) + 3];

            std::array<std::reference_wrapper<T>, ColumnNumV> a {
                e0,
                e1,
                e2,
                e3
            };
            return a;
        }
        static std::array<std::reference_wrapper<const T>, ColumnNumV> get(const std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            const T& e0 = components[(index * ColumnNumV) + 0];
            const T& e1 = components[(index * ColumnNumV) + 1];
            const T& e2 = components[(index * ColumnNumV) + 2];
            const T& e3 = components[(index * ColumnNumV) + 3];

            std::array<std::reference_wrapper<const T>, ColumnNumV> a {
                e0,
                e1,
                e2,
                e3
            };
            return a;
        }
    };

    // ColumnHelper

    template <int RowNumV, int ColumnNumV>
    struct ColumnHelper {
        // static inline constexpr int Num = 0;
        static std::array<std::reference_wrapper<T>, RowNumV> get(std::array<T, RowNumV * ColumnNumV>& a, int index)
        {
            throw std::logic_error("not implemented");
        }
        static std::array<std::reference_wrapper<const T>, RowNumV> get(const std::array<T, RowNumV * ColumnNumV>& a, int index)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <int ColumnNumV>
    struct ColumnHelper<2, ColumnNumV> {
        static inline constexpr int RowNumV = 2;
        static std::array<std::reference_wrapper<T>, RowNumV> get(std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            T& e0 = components[index];
            T& e1 = components[(1 * ColumnNumV) + index + 0];
            // T& e2 = components[(2 * ColumnNumV) + index + 0];
            // T& e3 = components[(3 * ColumnNumV) + index + 0];

            std::array<std::reference_wrapper<T>, RowNumV> a {
                e0,
                e1,
                // e2,
                // e3
            };
            return a;
        }
        static std::array<std::reference_wrapper<const T>, RowNumV> get(const std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            const T& e0 = components[index];
            const T& e1 = components[(1 * ColumnNumV) + index + 0];
            // const T& e2 = components[(2 * ColumnNumV) + index + 0];
            // const T& e3 = components[(3 * ColumnNumV) + index + 0];

            std::array<std::reference_wrapper<const T>, RowNumV> a {
                e0,
                e1,
                // e2,
                // e3
            };
            return a;
        }
    };

    template <int ColumnNumV>
    struct ColumnHelper<3, ColumnNumV> {
        static inline constexpr int RowNumV = 3;
        static std::array<std::reference_wrapper<T>, RowNumV> get(std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            T& e0 = components[index];
            T& e1 = components[(1 * ColumnNumV) + index + 0];
            T& e2 = components[(2 * ColumnNumV) + index + 0];
            // T& e3 = components[(3 * ColumnNumV) + index + 0];

            std::array<std::reference_wrapper<T>, RowNumV> a {
                e0,
                e1,
                e2,
                // e3
            };
            return a;
        }
        static std::array<std::reference_wrapper<const T>, RowNumV> get(const std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            const T& e0 = components[index];
            const T& e1 = components[(1 * ColumnNumV) + index + 0];
            const T& e2 = components[(2 * ColumnNumV) + index + 0];
            // const T& e3 = components[(3 * ColumnNumV) + index + 0];

            std::array<std::reference_wrapper<const T>, RowNumV> a {
                e0,
                e1,
                e2,
                // e3
            };
            return a;
        }
    };

    template <int ColumnNumV>
    struct ColumnHelper<4, ColumnNumV> {
        static inline constexpr int RowNumV = 4;
        static std::array<std::reference_wrapper<T>, RowNumV> get(std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            T& e0 = components[index];
            T& e1 = components[(1 * ColumnNumV) + index + 0];
            T& e2 = components[(2 * ColumnNumV) + index + 0];
            T& e3 = components[(3 * ColumnNumV) + index + 0];

            std::array<std::reference_wrapper<T>, RowNumV> a {
                e0,
                e1,
                e2,
                e3
            };
            return a;
        }
        static std::array<std::reference_wrapper<const T>, RowNumV> get(const std::array<T, RowNumV * ColumnNumV>& components, int index)
        {
            const T& e0 = components[index];
            const T& e1 = components[(1 * ColumnNumV) + index + 0];
            const T& e2 = components[(2 * ColumnNumV) + index + 0];
            const T& e3 = components[(3 * ColumnNumV) + index + 0];

            std::array<std::reference_wrapper<const T>, RowNumV> a {
                e0,
                e1,
                e2,
                e3
            };
            return a;
        }
    };

    // TranslateHelper

    template <int RowNumV, int ColumnNumV>
    struct TranslateHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(VectorT<T, 3> v)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct TranslateHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> compute(VectorT<T, 3> v)
        {
            MatrixT<T, RowNumV, ColumnNumV> m;
            m.orderedAt(0, 3) = v.x();
            m.orderedAt(1, 3) = v.y();
            m.orderedAt(2, 3) = v.z();
            return m;
        }
    };

    // RotateHelper

    template <int RowNumV, int ColumnNumV>
    struct RotateHelper {
        static MatrixT<T, RowNumV, ColumnNumV> computeX(T angle)
        {
            throw std::logic_error("not implemented");
        }
        static MatrixT<T, RowNumV, ColumnNumV> computeY(T angle)
        {
            throw std::logic_error("not implemented");
        }
        static MatrixT<T, RowNumV, ColumnNumV> computeZ(T angle)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct RotateHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> computeX(T angle)
        {
            MatrixT<T, RowNumV, ColumnNumV> m;
            m.orderedAt(1, 1) = Math::cos(angle);
            m.orderedAt(1, 2) = -Math::sin(angle);
            m.orderedAt(2, 1) = Math::sin(angle);
            m.orderedAt(2, 2) = Math::cos(angle);
            return m;
        }
        static MatrixT<T, RowNum, ColumnNumV> computeY(T angle)
        {
            MatrixT<T, RowNumV, ColumnNumV> m;
            m.orderedAt(0, 0) = Math::cos(angle);
            m.orderedAt(0, 2) = Math::sin(angle);
            m.orderedAt(2, 0) = -Math::sin(angle);
            m.orderedAt(2, 2) = Math::cos(angle);
            return m;
        }
        static MatrixT<T, RowNum, ColumnNumV> computeZ(T angle)
        {
            MatrixT<T, RowNumV, ColumnNumV> m;
            m.orderedAt(0, 0) = Math::cos(angle);
            m.orderedAt(0, 1) = -Math::sin(angle);
            m.orderedAt(1, 0) = Math::sin(angle);
            m.orderedAt(1, 1) = Math::cos(angle);
            return m;
        }
    };

    // ScaleHelper

    template <int RowNumV, int ColumnNumV>
    struct ScaleHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(VectorT<T, 3> v)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct ScaleHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> compute(VectorT<T, 3> v)
        {
            MatrixT<T, RowNumV, ColumnNumV> m;
            m.orderedAt(0, 0) = v.x();
            m.orderedAt(1, 1) = v.y();
            m.orderedAt(2, 2) = v.z();
            return m;
        }
    };

    // InverseHelper

    template <int RowNumV, int ColumnNumV>
    struct InverseHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(const MatrixT<T, RowNumV, ColumnNumV>& m)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct InverseHelper<2, 2> {
        static inline constexpr int RowNumV = 2;
        static inline constexpr int ColumnNumV = 2;
        static MatrixT<T, RowNumV, ColumnNumV> compute(const MatrixT<T, RowNumV, ColumnNumV>& m)
        {
            MatrixT<T, RowNumV, ColumnNumV> ret;
            T a = m.orderedAt(0, 0);
            T b = m.orderedAt(0, 1);
            T c = m.orderedAt(1, 0);
            T d = m.orderedAt(1, 1);
            T scale = static_cast<T>(1) / ((a * d) - (b * c));
            ret.orderedAt(0, 0) = scale * d;
            ret.orderedAt(0, 1) = scale * -b;
            ret.orderedAt(1, 0) = scale * -c;
            ret.orderedAt(1, 1) = scale * a;
            return ret;
        }
    };

    template <>
    struct InverseHelper<3, 3> {
        static inline constexpr int RowNumV = 3;
        static inline constexpr int ColumnNumV = 3;
        static MatrixT<T, RowNumV, ColumnNumV> compute(const MatrixT<T, RowNumV, ColumnNumV>& m)
        {
            // see: https://risalc.info/src/inverse-cofactor-ex3.html
            MatrixT<T, RowNumV, ColumnNumV> ret;
            T a11 = m.orderedAt(0, 0);
            T a12 = m.orderedAt(0, 1);
            T a13 = m.orderedAt(0, 2);
            T a21 = m.orderedAt(1, 0);
            T a22 = m.orderedAt(1, 1);
            T a23 = m.orderedAt(1, 2);
            T a31 = m.orderedAt(2, 0);
            T a32 = m.orderedAt(2, 1);
            T a33 = m.orderedAt(2, 2);
            T scale = static_cast<T>(1) / ((a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32) - (a13 * a22 * a31) - (a12 * a21 * a33) - (a11 * a23 * a32));
            ret.orderedAt(0, 0) = scale * (a22 * a33 - a23 * a32);
            ret.orderedAt(0, 1) = scale * -(a12 * a33 - a13 * a32);
            ret.orderedAt(0, 2) = scale * (a12 * a23 - a13 * a22);
            ret.orderedAt(1, 0) = scale * -(a21 * a33 - a23 * a31);
            ret.orderedAt(1, 1) = scale * (a11 * a33 - a13 * a31);
            ret.orderedAt(1, 2) = scale * -(a11 * a23 - a13 * a21);
            ret.orderedAt(2, 0) = scale * (a21 * a32 - a22 * a31);
            ret.orderedAt(2, 1) = scale * -(a11 * a32 - a12 * a31);
            ret.orderedAt(2, 2) = scale * (a11 * a22 - a12 * a21);
            return ret;
        }
    };

    template <>
    struct InverseHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> compute(const MatrixT<T, RowNumV, ColumnNumV>& m)
        {
            // see: https://risalc.info/src/inverse-cofactor-ex4.html
            MatrixT<T, RowNumV, ColumnNumV> ret;
            T a11 = m.orderedAt(0, 0);
            T a12 = m.orderedAt(0, 1);
            T a13 = m.orderedAt(0, 2);
            T a14 = m.orderedAt(0, 3);
            T a21 = m.orderedAt(1, 0);
            T a22 = m.orderedAt(1, 1);
            T a23 = m.orderedAt(1, 2);
            T a24 = m.orderedAt(1, 3);
            T a31 = m.orderedAt(2, 0);
            T a32 = m.orderedAt(2, 1);
            T a33 = m.orderedAt(2, 2);
            T a34 = m.orderedAt(2, 3);
            T a41 = m.orderedAt(3, 0);
            T a42 = m.orderedAt(3, 1);
            T a43 = m.orderedAt(3, 2);
            T a44 = m.orderedAt(3, 3);
            T scale = static_cast<T>(1) / ((a11 * a22 * a33 * a44) + (a11 * a23 * a34 * a42) + (a11 * a24 * a32 * a43) -

                          (a11 * a24 * a33 * a42) - (a11 * a23 * a32 * a44) - (a11 * a22 * a32 * a43) +

                          (a12 * a21 * a33 * a44) - (a13 * a21 * a34 * a42) - (a14 * a21 * a32 * a43) +

                          (a14 * a21 * a33 * a42) + (a13 * a21 * a34 * a44) + (a12 * a21 * a34 * a43) +

                          (a12 * a23 * a31 * a44) + (a13 * a24 * a31 * a42) + (a14 * a22 * a31 * a43) -

                          (a14 * a23 * a31 * a42) - (a13 * a22 * a31 * a44) - (a12 * a24 * a31 * a43) -

                          (a12 * a23 * a34 * a41) - (a13 * a24 * a32 * a41) - (a14 * a22 * a33 * a41) +

                          (a14 * a23 * a32 * a41) + (a13 * a22 * a34 * a41) + (a12 * a24 * a33 * a41));
            ret.orderedAt(0, 0) = scale * ((a22 * a33 * a44) + (a23 * a34 * a42) + (a24 * a32 * a43) - (a24 * a33 * a42) - (a23 * a32 * a44) - (a22 * a34 * a43));
            ret.orderedAt(0, 1) = scale * ((-a12 * a33 * a44) - (a13 * a34 * a42) - (a14 * a32 * a43) + (a14 * a33 * a42) + (a13 * a32 * a44) + (a12 * a34 * a43));
            ret.orderedAt(0, 2) = scale * ((a12 * a23 * a44) + (a13 * a24 * a42) + (a14 * a22 * a43) - (a14 * a23 * a42) - (a13 * a22 * a44) - (a12 * a24 * a43));
            ret.orderedAt(0, 3) = scale * ((-a12 * a23 * a34) - (a13 * a24 * a32) - (a14 * a22 * a33) + (a14 * a23 * a32) + (a13 * a22 * a34) + (a12 * a24 * a33));

            ret.orderedAt(1, 0) = scale * ((-a21 * a33 * a44) - (a23 * a34 * a41) - (a24 * a31 * a43) + (a24 * a33 * a41) + (a23 * a31 * a44) + (a21 * a34 * a43));
            ret.orderedAt(1, 1) = scale * ((a11 * a33 * a44) + (a13 * a34 * a41) + (a14 * a31 * a43) - (a14 * a33 * a41) - (a13 * a31 * a44) - (a11 * a34 * a43));
            ret.orderedAt(1, 2) = scale * ((-a11 * a23 * a44) - (a13 * a24 * a41) - (a14 * a21 * a43) + (a14 * a23 * a41) + (a13 * a21 * a44) + (a11 * a24 * a43));
            ret.orderedAt(1, 3) = scale * ((a11 * a23 * a34) + (a13 * a24 * a31) + (a14 * a21 * a33) - (a14 * a23 * a31) - (a13 * a21 * a34) - (a11 * a24 * a33));

            ret.orderedAt(2, 0) = scale * ((a21 * a32 * a44) + (a22 * a34 * a41) + (a24 * a31 * a42) - (a24 * a32 * a41) - (a22 * a31 * a44) - (a21 * a34 * a42));
            ret.orderedAt(2, 1) = scale * ((-a11 * a32 * a44) - (a12 * a34 * a41) - (a14 * a31 * a42 + (a14 * a32 * a41) + (a12 * a31 * a44) + (a11 * a34 * a42)));
            ret.orderedAt(2, 2) = scale * ((a11 * a22 * a44) + (a12 * a24 * a41) + (a14 * a21 * a42) - (a14 * a22 * a41 - (a12 * a21 * a44) - (a11 * a24 * a42)));
            ret.orderedAt(2, 3) = scale * ((-a11 * a22 * a34) - (a12 * a24 * a31) - (a14 * a21 * a32) + (a14 * a22 * a31) + (a12 * a21 * a34) + (a11 * a24 * a32));

            ret.orderedAt(3, 0) = scale * ((-a21 * a32 * a43) - (a22 * a33 * a41) - (a23 * a31 * a42) + (a23 * a32 * a41) + (a22 * a31 * a43) + (a21 * a33 * a42));
            ret.orderedAt(3, 1) = scale * ((a11 * a32 * a43) + (a12 * a33 * a41) + (a13 * a31 * a42) - (a13 * a32 * a41) - (a12 * a31 * a43) - (a11 * a33 * a42));
            ret.orderedAt(3, 2) = scale * ((-a11 * a22 * a43) - (a12 * a23 * a41) - (a13 * a21 * a42) + (a13 * a22 * a41 + (a12 * a21 * a43) + (a11 * a23 * a42)));
            ret.orderedAt(3, 3) = scale * ((a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32) - (a13 * a22 * a31) - (a12 * a21 * a33) - (a11 * a23 * a32));
            return ret;
        }
    };

    // OrthoHelper

    template <int RowNumV, int ColumnNumV>
    struct OrthoHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(T left, T top, T right, T bottom, T zNear, T zFar)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct OrthoHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> compute(T left, T top, T right, T bottom, T zNear, T zFar)
        {
            MatrixT<T, RowNumV, ColumnNumV> ret;
            T two = static_cast<T>(2);
#if MATH_MATRIX_HAND == MATH_MATRIX_RH
            ret.at(0, 0) = two / (right - left);
            ret.at(1, 1) = two / (top - bottom);
            ret.at(2, 2) = -two / (zFar - zNear);
            ret.at(3, 0) = -(right + left) / (right - left);
            ret.at(3, 1) = -(top + bottom) / (top - bottom);
            ret.at(3, 2) = -(zFar + zNear) / (zFar - zNear);
#endif
            return ret;
        }
    };

    // Perspective

    template <int RowNumV, int ColumnNumV>
    struct PerspectiveHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(T fovy, T aspectRatio, T zNear, T zFar)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct PerspectiveHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> compute(T fovy, T aspectRatio, T zNear, T zFar)
        {
            MatrixT<T, RowNumV, ColumnNumV> ret = MatrixT<T, RowNumV, ColumnNumV>::filled(static_cast<T>(0));
            T tanHalfFovy = static_cast<T>(std::tan(fovy / static_cast<T>(2)));
            T one = static_cast<T>(1);
#if MATH_MATRIX_HAND == MATH_MATRIX_RH
            ret.at(0, 0) = one / (aspectRatio * tanHalfFovy);
            ret.at(1, 1) = one / (tanHalfFovy);
            ret.at(2, 2) = -(zFar + zNear) / (zFar - zNear);
            ret.at(2, 3) = -(one);
            ret.at(3, 2) = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
#endif
            return ret;
        }
    };

    // LookAt

    template <int RowNumV, int ColumnNumV>
    struct LookAtHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(const VectorT<T, 3>& eye, const VectorT<T, 3>& at, const VectorT<T, 3>& up)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct LookAtHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;
        static MatrixT<T, RowNumV, ColumnNumV> compute(const VectorT<T, 3>& eye, const VectorT<T, 3>& at, const VectorT<T, 3>& up)
        {
            VectorT<T, 3> f = VectorT<T, 3>::normalized(at - eye);
            VectorT<T, 3> s = VectorT<T, 3>::normalized(VectorT<T, 3>::cross(f, up));
            VectorT<T, 3> u = VectorT<T, 3>::cross(s, f);
            MatrixT<T, 4, 4> ret;
#if MATH_MATRIX_HAND == MATH_MATRIX_RH
            ret.at(0, 0) = s.x();
            ret.at(1, 0) = s.y();
            ret.at(2, 0) = s.z();
            ret.at(0, 1) = u.x();
            ret.at(1, 1) = u.y();
            ret.at(2, 1) = u.z();
            ret.at(0, 2) = -f.x();
            ret.at(1, 2) = -f.y();
            ret.at(2, 2) = -f.z();
            ret.at(3, 0) = -VectorT<T, 3>::dot(s, eye);
            ret.at(3, 1) = -VectorT<T, 3>::dot(u, eye);
            ret.at(3, 2) = VectorT<T, 3>::dot(f, eye);
#endif
            return ret;
        }
    };

    // Transpose

    template <int RowNumV, int ColumnNumV>
    struct TransposeHelper {
        static MatrixT<T, RowNumV, ColumnNumV> compute(const MatrixT<T, RowNumV, ColumnNumV>& m)
        {
            throw std::logic_error("not implemented");
        }
    };

    template <>
    struct TransposeHelper<4, 4> {
        static inline constexpr int RowNumV = 4;
        static inline constexpr int ColumnNumV = 4;

        static MatrixT<T, RowNumV, ColumnNumV> compute(const MatrixT<T, RowNumV, ColumnNumV>& m)
        {
            MatrixT<T, RowNumV, ColumnNumV> ret;
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
    };

    // mutable

    std::array<std::reference_wrapper<T>, ColumnNum> row(int index)
    {
        return RowHelper<RowNum, ColumnNum>::get(this->components, index);
    }

    std::array<std::reference_wrapper<T>, RowNum> column(int index)
    {
        return ColumnHelper<RowNum, ColumnNum>::get(this->components, index);
    }

    T& at(int row, int col)
    {
        return components[(row * ColumnNum) + col];
    }

    T& orderedAt(int row, int col)
    {
#if MATH_MATRIX_MODE == MATH_MATRIX_COLUMN_MAJOR
        return at(col, row);
#else
        return at(row, col);
#endif
    }

    // immutable

    std::array<std::reference_wrapper<const T>, ColumnNum> row(int index) const
    {
        return RowHelper<RowNum, ColumnNum>::get(this->components, index);
    }

    std::array<std::reference_wrapper<const T>, RowNum> column(int index) const
    {
        return ColumnHelper<RowNum, ColumnNum>::get(this->components, index);
    }

    T at(int row, int col) const
    {
        return components[(row * ColumnNum) + col];
    }

    T orderedAt(int row, int col) const
    {
#if MATH_MATRIX_MODE == MATH_MATRIX_COLUMN_MAJOR
        return at(col, row);
#else
        return at(row, col);
#endif
    }

    // operator

    const std::array<std::reference_wrapper<const T>, ColumnNum> operator[](std::size_t i) const
    {
        return row(static_cast<int>(i));
    }
    std::array<std::reference_wrapper<T>, ColumnNum> operator[](std::size_t i)
    {
        return row(static_cast<int>(i));
    }

    // utility

    template <int NewNum>
    static MatrixT<T, RowNum, NewNum> multiply(const MatrixT<T, RowNum, ColumnNum>& a, const MatrixT<T, ColumnNum, NewNum>& b)
    {
        MatrixT<T, RowNum, NewNum> m;
        for (int i = 0; i < RowNum; i++) {
            for (int j = 0; j < NewNum; j++) {
                std::array<std::reference_wrapper<const T>, ColumnNum> row = a.row(i);
                std::array<std::reference_wrapper<const T>, RowNum> col = b.column(j);
                T sum = static_cast<T>(0);
                for (int k = 0; k < RowNum; k++) {
                    sum += row[k] * col[k];
                }
                m.at(i, j) = sum;
            }
        }
        return m;
    }

    static MatrixT<T, RowNum, ColumnNum> translate(VectorT<T, 3> v)
    {
        return TranslateHelper<RowNum, ColumnNum>::compute(v);
    }

    static MatrixT<T, RowNum, ColumnNum> rotateX(T angle)
    {
        return RotateHelper<RowNum, ColumnNum>::computeX(angle);
    }

    static MatrixT<T, RowNum, ColumnNum> rotateY(T angle)
    {
        return RotateHelper<RowNum, ColumnNum>::computeY(angle);
    }

    static MatrixT<T, RowNum, ColumnNum> rotateZ(T angle)
    {
        return RotateHelper<RowNum, ColumnNum>::computeZ(angle);
    }

    static MatrixT<T, RowNum, ColumnNum> scale(VectorT<T, 3> v)
    {
        return ScaleHelper<RowNum, ColumnNum>::compute(v);
    }

    static MatrixT<T, RowNum, ColumnNum> inverse(const MatrixT<T, RowNum, ColumnNum>& m)
    {
        return InverseHelper<RowNum, ColumnNum>::compute(m);
    }

    static MatrixT<T, RowNum, ColumnNum> ortho(T left, T top, T right, T bottom, T zNear, T zFar)
    {
        return OrthoHelper<RowNum, ColumnNum>::compute(left, top, right, bottom, zNear, zFar);
    }

    static MatrixT<T, RowNum, ColumnNum> perspective(T fovy, T aspectRatio, T zNear, T zFar)
    {
        return PerspectiveHelper<RowNum, ColumnNum>::compute(fovy, aspectRatio, zNear, zFar);
    }

    static MatrixT<T, RowNum, ColumnNum> lookAt(const VectorT<T, 3>& eye, const VectorT<T, 3>& at, const VectorT<T, 3>& up)
    {
        return LookAtHelper<RowNum, ColumnNum>::compute(eye, at, up);
    }

    static MatrixT<T, RowNum, ColumnNum> filled(T v)
    {
        MatrixT<T, RowNum, ColumnNum> ret;
        std::fill(ret.components.begin(), ret.components.end(), v);
        return ret;
    }

    static MatrixT<T, RowNum, ColumnNum> transpose(const MatrixT<T, RowNum, ColumnNum>& m)
    {
        return TransposeHelper<RowNum, ColumnNum>::compute(m);
    }

    std::string str() const
    {
        std::stringstream ss;
        ss << std::endl;
        for (int i = 0; i < RowNum; i++) {
            int start = i * ColumnNum;
            int end = start + ColumnNum;
            ss << "[";
            for (int n = start; n < end; n++) {
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

    std::array<T, RowNum * ColumnNum> components;
};
template <typename T, int RowNum, int ColumnNum>
bool operator==(const MatrixT<T, RowNum, ColumnNum>& a, const MatrixT<T, RowNum, ColumnNum>& b)
{
    for (int i = 0; i < RowNum * ColumnNum; i++) {
        if (!Math::equals(a.components[i], b.components[i])) {
            return false;
        }
    }
    return true;
}

template <typename T, int RowNum, int ColumnNum>
bool operator!=(const MatrixT<T, RowNum, ColumnNum>& a, const MatrixT<T, RowNum, ColumnNum>& b)
{
    return !(a == b);
}

template <typename T, int RowNum, int ColumnNum, int NewNum>
MatrixT<T, RowNum, NewNum> operator*(const MatrixT<T, ColumnNum, NewNum>& b, const MatrixT<T, RowNum, ColumnNum>& a)
{
    return MatrixT<T, RowNum, ColumnNum>::template multiply<NewNum>(a, b);
}

using Matrix2x2 = MatrixT<float, 2, 2>;
using Matrix2x3 = MatrixT<float, 2, 3>;
using Matrix2x4 = MatrixT<float, 2, 4>;
using Matrix3x2 = MatrixT<float, 3, 2>;
using Matrix3x3 = MatrixT<float, 3, 3>;
using Matrix3x4 = MatrixT<float, 3, 4>;
using Matrix4x2 = MatrixT<float, 4, 2>;
using Matrix4x3 = MatrixT<float, 4, 3>;
using Matrix4x4 = MatrixT<float, 4, 4>;
}
