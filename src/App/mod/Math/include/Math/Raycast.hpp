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
            float x1 = alignTile(offset.x(), tileSize);
            float x2 = alignTile(end.x(), tileSize);
            float y1 = alignTile(offset.y(), tileSize);
            float y2 = alignTile(end.y(), tileSize);
            float z1 = alignTile(offset.z(), tileSize);
            float z2 = alignTile(end.z(), tileSize);

            float minX = Mathf::min(x1, x2);
            float maxX = Mathf::max(x1, x2);
            float minY = Mathf::min(y1, y2);
            float maxY = Mathf::max(y1, y2);
            float minZ = Mathf::min(z1, z2);
            float maxZ = Mathf::max(z1, z2);

            for (float x = minX; x <= maxX; x += tileSize) {
                for (float y = minY; y <= maxY; y += tileSize) {
                    for (float z = minZ; z <= maxZ; z += tileSize) {
                        HitTile hitTile;
                        hitTile.position = Vector3({ x, y, z });

                        Vector3 faceTop = Vector3({ x, y + tileHalf, z });
                        if (offset.y() > faceTop.y() && faceTop.y() > end.y()) {
                            hitTile.normal = Vector3({ 0, 1, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceBottom = Vector3({ x, y - tileHalf, z });
                        if (offset.y() < faceBottom.y() && faceBottom.y() < end.y()) {
                            hitTile.normal = Vector3({ 0, -1, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceLeft = Vector3({ x - tileHalf, y, z });
                        if (offset.x() < faceLeft.x() && faceLeft.x() < end.x()) {
                            hitTile.normal = Vector3({ -1, 0, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceRight = Vector3({ x + tileHalf, y, z });
                        if (offset.x() > faceRight.x() && faceRight.x() > end.x()) {
                            hitTile.normal = Vector3({ 1, 0, 0 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceForward = Vector3({ x, y, z + tileHalf });
                        if (offset.z() > faceForward.z() && faceForward.z() > end.z()) {
                            hitTile.normal = Vector3({ 0, 0, 1 });
                            vec.push_back(hitTile);
                        }

                        Vector3 faceBackward = Vector3({ x, y, z - tileHalf });
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

    inline static float alignTile(float a, float tileSize = 1.0f)
    {
        float tileHalf = tileSize / 2.0f;
        float d = a / tileSize;
        if (d > 0.0f) {
            d = ::floorf(d);
        } else {
            d = ::ceilf(d);
        }
        float m = ::fmodf(a, tileSize);

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
};
}