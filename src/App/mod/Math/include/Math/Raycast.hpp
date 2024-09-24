#pragma once
#include <Math/Mathf.hpp>
#include <Math/Vector.hpp>
#include <cmath>
#include <vector>

namespace Lib::Math {
class Raycast {
public:
    class HitTile {
    public:
        explicit HitTile() = default;
        Vector3 position;
        Vector3 normal;
    };

    inline static std::vector<HitTile> testTiles(const Vector3& origin, const Vector3& dir, float length, float tileSize = 1.0f)
    {
        std::vector<HitTile> vec;
        Vector3 offset = origin;
        float distance = 0.0f;
        float tileHalf = tileSize / 2.0f;
        while (distance < length) {
            Vector3 end = offset + (dir * tileSize);
            float x1 = alignTile(offset.x());
            float x2 = alignTile(end.x());
            float y1 = alignTile(offset.y());
            float y2 = alignTile(end.y());
            float z1 = alignTile(offset.z());
            float z2 = alignTile(end.z());

            float minX = Mathf::min(x1, x2);
            float maxX = Mathf::max(x1, x2);
            float minY = Mathf::min(y1, y2);
            float maxY = Mathf::max(y1, y2);
            float minZ = Mathf::min(z1, z2);
            float maxZ = Mathf::max(z1, z2);

            for (float x = minX; x <= maxX; x += tileSize) {
                for (float y = minY; y <= maxY; y += tileSize) {
                    for (float z = minZ; z <= maxZ; z += tileSize) {
                        float fx = static_cast<float>(x);
                        float fy = static_cast<float>(y);
                        float fz = static_cast<float>(z);
                        HitTile hitTile;
                        hitTile.position = Vector3({ fx, fy, fz });

                        Vector3 faceTop = Vector3({ fx, fy + tileHalf, fz });
                        if (offset.y() > faceTop.y() && faceTop.y() > end.y()) {
                            hitTile.normal = Vector3({ 0, 1, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceBottom = Vector3({ fx, fy - tileHalf, fz });
                        if (offset.y() < faceBottom.y() && faceBottom.y() < end.y()) {
                            hitTile.normal = Vector3({ 0, -1, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceLeft = Vector3({ fx - tileHalf, fy, fz });
                        if (offset.x() < faceLeft.x() && faceLeft.x() < end.x()) {
                            hitTile.normal = Vector3({ -1, 0, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceRight = Vector3({ fx + tileHalf, fy, fz });
                        if (offset.x() > faceRight.x() && faceRight.x() > end.x()) {
                            hitTile.normal = Vector3({ 1, 0, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceForward = Vector3({ fx, fy, fz + tileHalf });
                        if (offset.z() > faceForward.z() && faceForward.z() > end.z()) {
                            hitTile.normal = Vector3({ 0, 0, 1 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceBackward = Vector3({ fx, fy, fz - tileHalf });
                        if (offset.z() < faceBackward.z() && faceBackward.z() < end.z()) {
                            hitTile.normal = Vector3({ 0, 0, -1 });
                            vec.push_back(hitTile);
                        }
                    }
                }
            }

            offset = end;
            distance += tileSize;
        }
        return vec;
    }

    inline static float alignTile(float a, float tileSize = 1.0f)
    {
        // a = -0.6
        // tileSize = 0.5
        float tileHalf = tileSize / 2.0f; // 0.25
        float d = a / tileSize; // -1
        if (d > 0.0f) {
            d = ::floorf(d);
        } else {
            d = ::ceilf(d);
        }
        float m = ::fmodf(a, tileSize); // -0.1

        if (::fabs(m) < 0.000001f) {
            return a;
        }
        if (m > 0.0f) {
            if (m < tileHalf) {
                return d * tileSize;
            }
            return (d * tileSize) + tileSize;
        } else {
            if (::fabs(m) < tileHalf) {
                return d * tileSize;
            }
            return (d * tileSize) - tileSize;
        }
    }

private:
    Raycast() = delete;
    ~Raycast() = delete;
};
}