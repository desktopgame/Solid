#include <Scenes/Game/System/Field.hpp>
#include <Scenes/Game/System/FieldGenerator.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace App::Scenes::Game::System {
// Room
FieldGenerator::Room::Room()
    : size()
    , center()
    , index()
    , linkTo()
    , isGarbage()
    , isNoLink()
{
}

bool FieldGenerator::Room::isContains(const IntVector3& at) const
{
    int32_t selfMinX = center.x() - (size.x() / 2);
    int32_t selfMaxX = center.x() + (size.x() / 2);
    int32_t selfMinZ = center.z() - (size.z() / 2);
    int32_t selfMaxZ = center.z() + (size.z() / 2);
    bool cX = at.x() >= selfMinX && at.x() < selfMaxX;
    bool cZ = at.z() >= selfMinZ && at.z() < selfMaxZ;
    return cX && cZ;
}
bool FieldGenerator::Room::isOverwrap(const Room& other) const
{
    int32_t selfMinX = center.x() - (size.x() / 2) - 5;
    int32_t selfMaxX = center.x() + (size.x() / 2) + 5;
    int32_t selfMinZ = center.z() - (size.z() / 2) - 5;
    int32_t selfMaxZ = center.z() + (size.z() / 2) + 5;
    int32_t otherMinX = other.center.x() - (other.size.x() / 2);
    int32_t otherMaxX = other.center.x() + (other.size.x() / 2);
    int32_t otherMinZ = other.center.z() - (other.size.z() / 2);
    int32_t otherMaxZ = other.center.z() + (other.size.z() / 2);

    bool overlapX = (selfMinX < otherMaxX && selfMaxX > otherMinX);
    bool overlapZ = (selfMinZ < otherMaxZ && selfMaxZ > otherMinZ);
    return overlapX && overlapZ;
}
// public
FieldGenerator::FieldGenerator() { }
// private
void FieldGenerator::generate()
{
    const int32_t k_sizeX = Field::k_fieldSizeX;
    const int32_t k_sizeY = Field::k_fieldSizeY;
    const int32_t k_sizeZ = Field::k_fieldSizeZ;
    (void)k_sizeY;

    std::vector<Room> rooms;
    Random rand;

    int32_t roomCount = rand.range(10, 20);
    // かさならない位置に部屋を作成
    for (int32_t i = 0; i < roomCount; i++) {
        Room room;
        int32_t roomSizeXZ = rand.range(10, 20);
        int32_t roomSizeY = rand.range(10, 20);

        if (roomSizeXZ % 2 == 0) {
            roomSizeXZ++;
        }
        room.size = IntVector3({ roomSizeXZ, roomSizeY, roomSizeXZ });

        std::vector<IntVector3> candidates;
        for (int32_t x = 0; x < k_sizeX; x++) {
            for (int32_t z = 0; z < k_sizeZ; z++) {
                room.center = IntVector3({ x, 0, z });

                int32_t minX = room.center.x() - (room.size.x() / 2);
                int32_t maxX = room.center.x() + (room.size.x() / 2);
                int32_t minZ = room.center.z() - (room.size.z() / 2);
                int32_t maxZ = room.center.z() + (room.size.z() / 2);

                bool retry = false;
                if (minX < 0 || minX >= k_sizeX) {
                    retry = true;
                }
                if (maxX < 0 || maxX >= k_sizeX) {
                    retry = true;
                }
                if (minZ < 0 || minZ >= k_sizeZ) {
                    retry = true;
                }
                if (maxZ < 0 || maxZ >= k_sizeZ) {
                    retry = true;
                }
                if (retry) {
                    continue;
                }

                for (const auto& otherRoom : rooms) {
                    if (room.isOverwrap(otherRoom)) {
                        retry = true;
                        break;
                    }
                }

                if (retry) {
                    continue;
                }
                candidates.emplace_back(room.center);
            }
        }

        if (!candidates.empty()) {
            room.center = candidates.at(rand.range(0, static_cast<int32_t>(candidates.size() - 1)));
            room.index = static_cast<int32_t>(rooms.size());
            rooms.emplace_back(room);
        }
    }

    // どの部屋につながるかを設定
    for (int32_t i = 0; i < static_cast<int32_t>(rooms.size()); i++) {
        int32_t linkTo = rand.range(0, static_cast<int32_t>(rooms.size() - 1));
        while (linkTo == i) {
            linkTo = rand.range(0, static_cast<int32_t>(rooms.size() - 1));
        }
        rooms.at(i).linkTo = linkTo;
    }

    // 孤立した部屋をマーク
    for (int32_t i = 0; i < static_cast<int32_t>(rooms.size()); i++) {
        auto& room = rooms.at(i);

        bool link = false;
        for (int32_t j = 0; j < static_cast<int32_t>(rooms.size()); j++) {
            if (i == j) {
                continue;
            }
            if (room.index == rooms.at(j).linkTo) {
                link = true;
                break;
            }
        }

        if (!link) {
            room.isGarbage = true;
        }
    }

    // 孤立した部屋を削除
    auto iter = std::remove_if(rooms.begin(), rooms.end(), [](const auto& e) -> bool {
        return e.isGarbage;
    });
    rooms.erase(iter, rooms.end());

    // お互いにリンクしていたら片方を非リンク
    for (int32_t i = 0; i < static_cast<int32_t>(rooms.size()); i++) {
        auto& room = rooms.at(i);

        for (int32_t j = 0; j < static_cast<int32_t>(rooms.size()); j++) {
            if (i == j) {
                continue;
            }
            auto& otherRoom = rooms.at(j);

            if (room.index == otherRoom.linkTo && room.linkTo == otherRoom.index) {
                if (!room.isNoLink && !otherRoom.isNoLink) {
                    int32_t n = rand.range(0, 1);
                    (n == 0 ? room : otherRoom).isNoLink = true;
                }
            }
        }
    }

    // ブロック座標を記憶する配列を確保
    std::array<std::array<std::array<bool, Field::k_fieldSizeZ>, Field::k_fieldSizeY>, Field::k_fieldSizeX> table;
    for (int32_t x = 0; x < k_sizeX; x++) {
        for (int32_t y = 0; y < k_sizeY; y++) {
            std::array<bool, Field::k_fieldSizeZ>& a = table[x][y];
            std::fill(a.begin(), a.end(), false);
        }
    }

    // 道を作成
    for (const auto& src : rooms) {
        // リンクしないなら無視
        if (src.isNoLink) {
            continue;
        }
        // 対応するルームを検索
        auto iter = std::find_if(rooms.begin(), rooms.end(), [src](const auto& e) -> bool {
            return src.linkTo == e.index && src.index != e.index;
        });
        assert(iter != rooms.end());
        const Room& dst = *iter;
        // 現在の部屋とリンク先の部屋の中心座標
        int32_t srcCenterX = src.center.x();
        int32_t srcCenterZ = src.center.z();
        int32_t srcSizeX = src.size.x();
        int32_t srcSizeZ = src.size.z();
        int32_t dstCenterX = dst.center.x();
        int32_t dstCenterZ = dst.center.z();
        int32_t dstSizeX = dst.size.x();
        int32_t dstSizeZ = dst.size.z();

        int32_t srcMinX = srcCenterX - (srcSizeX / 2);
        int32_t srcMaxX = srcCenterX + (srcSizeX / 2);
        int32_t srcMinZ = srcCenterZ - (srcSizeZ / 2);
        int32_t srcMaxZ = srcCenterZ + (srcSizeZ / 2);
        int32_t dstMinX = dstCenterX - (dstSizeX / 2);
        int32_t dstMaxX = dstCenterX + (dstSizeX / 2);
        int32_t dstMinZ = dstCenterZ - (dstSizeZ / 2);
        int32_t dstMaxZ = dstCenterZ + (dstSizeZ / 2);

        int32_t routeLength = 3;

        std::vector<IntVector3> srcRouteLeft;
        std::vector<IntVector3> srcRouteRight;
        std::vector<IntVector3> srcRouteTop;
        std::vector<IntVector3> srcRouteBottom;
        std::vector<IntVector3> dstRouteLeft;
        std::vector<IntVector3> dstRouteRight;
        std::vector<IntVector3> dstRouteTop;
        std::vector<IntVector3> dstRouteBottom;
        std::vector<IntVector3> fillArea;
        while (1) {
            // ソース部屋から通路を伸ばす
            srcRouteLeft.clear();
            srcRouteRight.clear();
            srcRouteTop.clear();
            srcRouteBottom.clear();
            for (int32_t i = 1; i < routeLength; i++) {
                if (srcMinX - i < 0) {
                    break;
                }
                srcRouteLeft.emplace_back(IntVector3({ srcMinX - i, 0, srcCenterZ }));
            }
            for (int32_t i = 1; i < routeLength; i++) {
                if (srcMaxX + i >= k_sizeX) {
                    break;
                }
                srcRouteRight.emplace_back(IntVector3({ srcMaxX + i, 0, srcCenterZ }));
            }
            for (int32_t i = 1; i < routeLength; i++) {
                if (srcMinZ - i < 0) {
                    break;
                }
                srcRouteTop.emplace_back(IntVector3({ srcCenterX, 0, srcMinZ - i }));
            }
            for (int32_t i = 1; i < routeLength; i++) {
                if (srcMaxZ + i >= k_sizeZ) {
                    break;
                }
                srcRouteBottom.emplace_back(IntVector3({ srcCenterX, 0, srcMaxZ + i }));
            }
            // デスト部屋から通路を伸ばす
            dstRouteLeft.clear();
            dstRouteRight.clear();
            dstRouteTop.clear();
            dstRouteBottom.clear();
            for (int32_t i = 1; i < routeLength; i++) {
                if (dstMinX - i < 0) {
                    break;
                }
                dstRouteLeft.emplace_back(IntVector3({ dstMinX - i, 0, dstCenterZ }));
            }
            for (int32_t i = 1; i < routeLength; i++) {
                if (dstMaxX + i >= k_sizeX) {
                    break;
                }
                dstRouteRight.emplace_back(IntVector3({ dstMaxX + i, 0, dstCenterZ }));
            }
            for (int32_t i = 1; i < routeLength; i++) {
                if (dstMinZ - i < 0) {
                    break;
                }
                dstRouteTop.emplace_back(IntVector3({ dstCenterX, 0, dstMinZ - i }));
            }
            for (int32_t i = 1; i < routeLength; i++) {
                if (dstMaxZ + i >= k_sizeZ) {
                    break;
                }
                dstRouteBottom.emplace_back(IntVector3({ dstCenterX, 0, dstMaxZ + i }));
            }

            bool connection = false;
            // 左でつなぐ
            // 道でつなぐ過程で他の部屋と重なる場合は接続しない
            if (!connection && !srcRouteLeft.empty() && !dstRouteLeft.empty()) {
                fillArea.clear();
                bool lineBroken = false;
                std::vector<IntVector3> line;
                IntVector3 head = srcRouteLeft.back();
                int32_t minZ = Mathf::min(srcCenterZ, dstCenterZ);
                int32_t maxZ = Mathf::max(srcCenterZ, dstCenterZ);

                for (int32_t at = minZ; at <= maxZ; at++) {
                    if (head.z() + (at - minZ) >= k_sizeZ) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x(), 0, head.z() + (at - minZ) }));
                }

                head = line.back();
                int32_t minX = Mathf::min(head.x(), dstRouteLeft.back().x());
                int32_t maxX = Mathf::max(head.x(), dstRouteLeft.back().x());
                for (int32_t at = minX; at <= maxX; at++) {
                    if (head.x() + (at - minX) >= k_sizeX) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x() + (at - minX), 0, head.z() }));
                }

                if (line.back() != dstRouteLeft.back()) {
                    lineBroken = true;
                }

                bool cross = false;
                if (!cross) {
                    for (const auto& at : srcRouteLeft) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : dstRouteLeft) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : line) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross && !line.empty() && !lineBroken) {
                    connection = true;
                    for (const auto& at : srcRouteLeft) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : dstRouteLeft) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : line) {
                        fillArea.emplace_back(at);
                    }
                }
            }
            // 右でつなぐ
            // 道でつなぐ過程で他の部屋と重なる場合は接続しない
            if (!connection && !srcRouteRight.empty() && !dstRouteRight.empty()) {
                fillArea.clear();
                bool lineBroken = false;
                std::vector<IntVector3> line;
                IntVector3 head = srcRouteRight.back();
                int32_t minZ = Mathf::min(srcCenterZ, dstCenterZ);
                int32_t maxZ = Mathf::max(srcCenterZ, dstCenterZ);

                for (int32_t at = minZ; at <= maxZ; at++) {
                    if (head.z() + (at - minZ) >= k_sizeZ) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x(), 0, head.z() + (at - minZ) }));
                }

                head = line.back();
                int32_t minX = Mathf::min(head.x(), dstRouteRight.back().x());
                int32_t maxX = Mathf::max(head.x(), dstRouteRight.back().x());
                for (int32_t at = minX; at <= maxX; at++) {
                    if (head.x() + (at - minX) >= k_sizeX) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x() + (at - minX), 0, head.z() }));
                }

                if (line.back() != dstRouteRight.back()) {
                    lineBroken = true;
                }

                bool cross = false;
                if (!cross) {
                    for (const auto& at : srcRouteRight) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : dstRouteRight) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : line) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross && !line.empty() && !lineBroken) {
                    connection = true;
                    for (const auto& at : srcRouteRight) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : dstRouteRight) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : line) {
                        fillArea.emplace_back(at);
                    }
                }
            }
            // 上でつなぐ
            // 道でつなぐ過程で他の部屋と重なる場合は接続しない
            if (!connection && !srcRouteTop.empty() && !dstRouteTop.empty()) {
                fillArea.clear();
                bool lineBroken = false;
                std::vector<IntVector3> line;
                IntVector3 head = srcRouteTop.back();
                int32_t minX = Mathf::min(srcCenterX, dstCenterX);
                int32_t maxX = Mathf::max(srcCenterX, dstCenterX);

                for (int32_t at = minX; at <= maxX; at++) {
                    if (head.x() + (at - minX) >= k_sizeX) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x() + (at - minX), 0, head.z() }));
                }

                head = line.back();
                int32_t minZ = Mathf::min(head.z(), dstRouteTop.back().z());
                int32_t maxZ = Mathf::max(head.z(), dstRouteTop.back().z());
                for (int32_t at = minZ; at <= maxZ; at++) {
                    if (head.z() + (at - minZ) >= k_sizeZ) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x(), 0, head.z() + (at - minZ) }));
                }

                if (line.back() != dstRouteTop.back()) {
                    lineBroken = true;
                }

                bool cross = false;
                if (!cross) {
                    for (const auto& at : srcRouteTop) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : dstRouteTop) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : line) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross && !line.empty() && !lineBroken) {
                    connection = true;
                    for (const auto& at : srcRouteTop) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : dstRouteTop) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : line) {
                        fillArea.emplace_back(at);
                    }
                }
            }
            // 下でつなぐ
            // 道でつなぐ過程で他の部屋と重なる場合は接続しない
            if (!connection && !srcRouteBottom.empty() && !dstRouteBottom.empty()) {
                fillArea.clear();
                bool lineBroken = false;
                std::vector<IntVector3> line;
                IntVector3 head = srcRouteBottom.back();
                int32_t minX = Mathf::min(srcCenterX, dstCenterX);
                int32_t maxX = Mathf::max(srcCenterX, dstCenterX);

                for (int32_t at = minX; at <= maxX; at++) {
                    if (head.x() + (at - minX) >= k_sizeX) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x() + (at - minX), 0, head.z() }));
                }

                head = line.back();
                int32_t minZ = Mathf::min(head.z(), dstRouteBottom.back().z());
                int32_t maxZ = Mathf::max(head.z(), dstRouteBottom.back().z());
                for (int32_t at = minZ; at <= maxZ; at++) {
                    if (head.z() + (at - minZ) >= k_sizeZ) {
                        lineBroken = true;
                        break;
                    }
                    line.emplace_back(IntVector3({ head.x(), 0, head.z() + (at - minZ) }));
                }

                if (line.back() != dstRouteBottom.back()) {
                    lineBroken = true;
                }

                bool cross = false;
                if (!cross) {
                    for (const auto& at : srcRouteBottom) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : dstRouteBottom) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross) {
                    for (const auto& at : line) {
                        for (const auto& room : rooms) {
                            if (room.isContains(at)) {
                                cross = true;
                                break;
                            }
                        }
                    }
                }
                if (!cross && !line.empty() && !lineBroken) {
                    connection = true;
                    for (const auto& at : srcRouteBottom) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : dstRouteBottom) {
                        fillArea.emplace_back(at);
                    }
                    for (const auto& at : line) {
                        fillArea.emplace_back(at);
                    }
                }
            }
            if (connection) {
                break;
            } else {
                routeLength++;
                if (routeLength > 100) {
                    break;
                }
            }
        }
        for (const auto& at : fillArea) {
            if (at.x() >= 0 && at.x() < k_sizeX && at.z() >= 0 && at.z() < k_sizeZ) {
                table[at.x()][0][at.z()] = true;
            }
        }
    }

    // ルームの床にブロックを配置
    for (const auto& room : rooms) {
        for (int32_t x = 0; x < room.size.x(); x++) {
            for (int32_t z = 0; z < room.size.z(); z++) {
                int32_t tileX = (room.center.x() - (room.size.x() / 2)) + x;
                int32_t tileZ = (room.center.z() - (room.size.z() / 2)) + z;
                table[tileX][0][tileZ] = true;
            }
        }
    }

    // 配置ブロック座標にタイルを六面分配置
    m_tiles.clear();
    for (int32_t x = 0; x < k_sizeX; x++) {
        for (int32_t y = 0; y < k_sizeY; y++) {
            for (int32_t z = 0; z < k_sizeZ; z++) {
                if (!table[x][y][z]) {
                    continue;
                }
                m_tiles.emplace_back(Vector4({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0f }));
                m_tiles.emplace_back(Vector4({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 1.0f }));
                m_tiles.emplace_back(Vector4({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 2.0f }));
                m_tiles.emplace_back(Vector4({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 3.0f }));
                m_tiles.emplace_back(Vector4({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 4.0f }));
                m_tiles.emplace_back(Vector4({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 5.0f }));
            }
        }
    }
}
const std::vector<Vector4>& FieldGenerator::getTiles() const { return m_tiles; }
}