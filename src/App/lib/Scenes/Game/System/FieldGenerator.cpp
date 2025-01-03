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
    , removed()
    , connected()
{
}

bool FieldGenerator::Room::isContains(const IntVector3& at) const
{
    int32_t selfMinX = center.x() - (size.x() / 2);
    int32_t selfMaxX = center.x() + (size.x() / 2);
    int32_t selfMinZ = center.z() - (size.z() / 2);
    int32_t selfMaxZ = center.z() + (size.z() / 2);
    bool cX = at.x() >= selfMinX && at.x() <= selfMaxX;
    bool cZ = at.z() >= selfMinZ && at.z() <= selfMaxZ;
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
    const int32_t k_wallPadding = 3;
    const int32_t k_roadLength = 5;
    (void)k_sizeY;

    const int32_t k_space = 4;
    int32_t roomSizeX = (Field::k_fieldSizeX - (k_space * 4)) / 3;
    int32_t roomSizeZ = (Field::k_fieldSizeZ - (k_space * 4)) / 3;

    std::vector<Room> rooms;
    Random rand;

    // 空間を9分割
    int32_t roomIndex = 0;
    for (int32_t i = 0; i < 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
            Room room;
            room.index = roomIndex;
            room.center = IntVector3({ ((i + 1) * k_space) + (roomSizeX * i) + (roomSizeX / 2), 0, ((j + 1) * k_space) + (roomSizeZ * j) + (roomSizeZ / 2) });
            room.size = IntVector3({ roomSizeX, Field::k_fieldSizeY, roomSizeZ });
            rooms.emplace_back(room);
            roomIndex++;
        }
    }

    // 部屋をランダムに消す
    int32_t removeN = rand.range(1, 3);
    for (int32_t i = 0; i < removeN; i++) {
        int32_t r = rand.range(0, static_cast<int32_t>(rooms.size() - 1));
        rooms.at(r).removed = true;
    }

    auto iter = std::remove_if(rooms.begin(), rooms.end(), [](const auto& e) -> bool {
        return e.removed;
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