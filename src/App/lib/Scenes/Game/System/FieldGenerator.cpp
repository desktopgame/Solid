#include <Scenes/Game/System/Field.hpp>
#include <Scenes/Game/System/FieldGenerator.hpp>
#include <algorithm>

namespace App::Scenes::Game::System {
// Room
FieldGenerator::Room::Room()
    : size()
    , center()
    , index()
    , linkTo()
    , isGarbage()
{
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
        int32_t linkTo = rand.range(0, static_cast<int32_t>(rooms.size()));
        while (linkTo == i) {
            linkTo = rand.range(0, static_cast<int32_t>(rooms.size()));
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

    // ブロック座標を記憶する配列を確保
    std::array<std::array<std::array<bool, Field::k_fieldSizeZ>, Field::k_fieldSizeY>, Field::k_fieldSizeX> table;
    for (int32_t x = 0; x < k_sizeX; x++) {
        for (int32_t y = 0; y < k_sizeY; y++) {
            std::array<bool, Field::k_fieldSizeZ>& a = table[x][y];
            std::fill(a.begin(), a.end(), false);
        }
    }

    // 道を作成
    for (const auto& room : rooms) {
        auto iter = std::find_if(rooms.begin(), rooms.end(), [room](const auto& e) -> bool {
            return room.linkTo == e.index;
        });
        const Room& targetRoom = *iter;

        // 現在の部屋とリンク先の部屋の中心座標
        int32_t startX = room.center.x();
        int32_t startZ = room.center.z();
        int32_t endX = targetRoom.center.x();
        int32_t endZ = targetRoom.center.z();

        // X方向に廊下を生成
        int32_t x = startX;
        while (x != endX && x >= 0 && x < k_sizeX) {
            table[x][0][startZ] = true;
            if (startZ - 1 >= 0) {
                table[x][0][startZ - 1] = true;
            }
            if (startZ + 1 < k_sizeZ) {
                table[x][0][startZ + 1] = true;
            }
            x += (x < endX) ? 1 : -1;
        }

        // Z方向に廊下を生成
        int32_t z = startZ;
        while (z != endZ && z >= 0 && z < k_sizeZ) {
            table[endX][0][z] = true;
            if (endX - 1 >= 0) {
                table[endX - 1][0][z] = true;
            }
            if (endX + 1 < k_sizeX) {
                table[endX + 1][0][z] = true;
            }
            z += (z < endZ) ? 1 : -1;
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