#pragma once
#include <Math/Geom/AABB.hpp>
#include <Math/Geom/OBB.hpp>
#include <Math/Mathf.hpp>
#include <Math/Vector.hpp>

namespace Lib::Math::Geom {
class Collision {
public:
    template <typename T>
    inline static bool testOBBAndOBB(const OBBT<T>& obb1, const OBBT<T>& obb2)
    {
        // see: http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
        // 各方向ベクトルの確保
        // （N***:標準化方向ベクトル）
        VectorT<T, 3> NAe1 = obb1.axes.at(0), Ae1 = NAe1 * obb1.size.at(0);
        VectorT<T, 3> NAe2 = obb1.axes.at(1), Ae2 = NAe2 * obb1.size.at(1);
        VectorT<T, 3> NAe3 = obb1.axes.at(2), Ae3 = NAe3 * obb1.size.at(2);
        VectorT<T, 3> NBe1 = obb2.axes.at(0), Be1 = NBe1 * obb2.size.at(0);
        VectorT<T, 3> NBe2 = obb2.axes.at(1), Be2 = NBe2 * obb2.size.at(1);
        VectorT<T, 3> NBe3 = obb2.axes.at(2), Be3 = NBe3 * obb2.size.at(2);
        VectorT<T, 3> Interval = obb1.center() - obb2.center();

        // 分離軸 : Ae1
        T rA = Ae1.length();
        T rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
        T L = Mathf::abs(VectorT<T, 3>::dot(Interval, NAe1));
        if (L > rA + rB)
            return false; // 衝突していない

        // 分離軸 : Ae2
        rA = Ae2.length();
        rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, NAe2));
        if (L > rA + rB)
            return false;

        // 分離軸 : Ae3
        rA = Ae3.length();
        rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, NAe3));
        if (L > rA + rB)
            return false;

        // 分離軸 : Be1
        rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
        rB = Be1.length();
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, NBe1));
        if (L > rA + rB)
            return false;

        // 分離軸 : Be2
        rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
        rB = Be2.length();
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, NBe2));
        if (L > rA + rB)
            return false;

        // 分離軸 : Be3
        rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
        rB = Be3.length();
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, NBe3));
        if (L > rA + rB)
            return false;

        // 分離軸 : C11
        VectorT<T, 3> Cross;
        Cross = VectorT<T, 3>::cross(NAe1, NBe1);
        rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C12
        Cross = VectorT<T, 3>::cross(NAe1, NBe2);
        rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C13
        Cross = VectorT<T, 3>::cross(NAe1, NBe3);
        rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
        rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C21
        Cross = VectorT<T, 3>::cross(NAe2, NBe1);
        rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C22
        Cross = VectorT<T, 3>::cross(NAe2, NBe2);
        rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C23
        Cross = VectorT<T, 3>::cross(NAe2, NBe3);
        rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
        rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C31
        Cross = VectorT<T, 3>::cross(NAe3, NBe1);
        rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C32
        Cross = VectorT<T, 3>::cross(NAe3, NBe2);
        rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離軸 : C33
        Cross = VectorT<T, 3>::cross(NAe3, NBe3);
        rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
        rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
        L = Mathf::abs(VectorT<T, 3>::dot(Interval, Cross));
        if (L > rA + rB)
            return false;

        // 分離平面が存在しないので「衝突している」
        return true;
    }

private:
    Collision() = delete;

    template <typename T>
    inline static T LenSegOnSeparateAxis(const VectorT<T, 3>& Sep, const VectorT<T, 3>& e1, const VectorT<T, 3>& e2, const VectorT<T, 3>* e3 = nullptr)
    {
        // see: http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
        T r1 = Mathf::abs(VectorT<T, 3>::dot(Sep, e1));
        T r2 = Mathf::abs(VectorT<T, 3>::dot(Sep, e2));
        T r3 = e3 ? (Mathf::abs(VectorT<T, 3>::dot(Sep, e3))) : 0;
        return r1 + r2 + r3;
    }
};
}