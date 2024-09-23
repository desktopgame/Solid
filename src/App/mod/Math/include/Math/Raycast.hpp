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
            int32_t x1 = static_cast<int32_t>(offset.x());
            int32_t x2 = static_cast<int32_t>(::roundf(end.x()));
            int32_t y1 = static_cast<int32_t>(offset.y());
            int32_t y2 = static_cast<int32_t>(::roundf(end.y()));
            int32_t z1 = static_cast<int32_t>(offset.z());
            int32_t z2 = static_cast<int32_t>(::roundf(end.z()));

            int32_t minX = Mathf::min(x1, x2);
            int32_t maxX = Mathf::max(x1, x2);
            int32_t minY = Mathf::min(y1, y2);
            int32_t maxY = Mathf::max(y1, y2);
            int32_t minZ = Mathf::min(z1, z2);
            int32_t maxZ = Mathf::max(z1, z2);

            for (int32_t x = minX; x <= maxX; x++) {
                for (int32_t y = minY; y <= maxY; y++) {
                    for (int32_t z = minZ; z <= maxZ; z++) {
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

private:
    Raycast() = delete;
    ~Raycast() = delete;
};
}