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
void FieldGenerator::generate()
{
    const int32_t k_sizeX = Field::k_fieldSizeX;
    const int32_t k_sizeY = Field::k_fieldSizeY;
    const int32_t k_sizeZ = Field::k_fieldSizeZ;
    const int32_t k_wallPadding = 3;
    const int32_t k_roadLength = 5;
    (void)k_sizeY;

    Random rand;
    const int32_t k_space = 4;
    int32_t roomSizeX = (Field::k_fieldSizeX - (k_space * 4)) / 3;
    int32_t roomSizeZ = (Field::k_fieldSizeZ - (k_space * 4)) / 3;

    m_rooms.clear();

    // 空間を9分割
    int32_t roomIndex = 0;
    for (int32_t j = 0; j < 3; j++) {
        for (int32_t i = 0; i < 3; i++) {
            Room room;
            room.index = roomIndex;
            room.center = IntVector3({ ((i + 1) * k_space) + (roomSizeX * i) + (roomSizeX / 2), 0, ((j + 1) * k_space) + (roomSizeZ * j) + (roomSizeZ / 2) });
            room.size = IntVector3({ roomSizeX, Field::k_fieldSizeY, roomSizeZ });
            m_rooms.emplace_back(room);
            roomIndex++;
        }
    }

    // 部屋をランダムに消す
    int32_t removeN = rand.range(2, 5);
    for (int32_t i = 0; i < removeN; i++) {
        int32_t r = rand.range(0, static_cast<int32_t>(m_rooms.size() - 1));
        m_rooms.at(r).removed = true;
    }

    auto iter = std::remove_if(m_rooms.begin(), m_rooms.end(), [](const auto& e) -> bool {
        return e.removed;
    });
    m_rooms.erase(iter, m_rooms.end());

    // ブロック座標を記憶する配列を確保
    std::array<std::array<std::array<int32_t, Field::k_fieldSizeZ>, Field::k_fieldSizeY>, Field::k_fieldSizeX> table;
    for (int32_t x = 0; x < k_sizeX; x++) {
        for (int32_t y = 0; y < k_sizeY; y++) {
            std::array<int32_t, Field::k_fieldSizeZ>& a = table[x][y];
            std::fill(a.begin(), a.end(), 0);
        }
    }

    // ルームの床にブロックを配置
    for (const auto& room : m_rooms) {
        for (int32_t x = 0; x < room.size.x(); x++) {
            for (int32_t z = 0; z < room.size.z(); z++) {
                int32_t tileX = (room.center.x() - (room.size.x() / 2)) + x;
                int32_t tileZ = (room.center.z() - (room.size.z() / 2)) + z;
                table[tileX][0][tileZ] = 1;
                table[tileX][Field::k_fieldSizeY - 1][tileZ] = 1;
            }
        }
        for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
            int32_t minX = (room.center.x() - (room.size.x() / 2));
            int32_t maxX = (room.center.x() + (room.size.x() / 2));
            int32_t minZ = (room.center.z() - (room.size.z() / 2));
            int32_t maxZ = (room.center.z() + (room.size.z() / 2));

            for (int32_t x = 0; x < room.size.x(); x++) {
                int32_t tileX = (room.center.x() - (room.size.x() / 2)) + x;
                table[tileX][y][minZ] = 1;
                table[tileX][y][maxZ] = 1;
            }
            for (int32_t z = 0; z < room.size.z(); z++) {
                int32_t tileZ = (room.center.z() - (room.size.z() / 2)) + z;
                table[minX][y][tileZ] = 1;
                table[maxX][y][tileZ] = 1;
            }
        }
    }

    // 通路を繋ぐ
    for (auto& room : m_rooms) {
        int32_t left = room.index - 1;
        int32_t right = room.index + 1;
        int32_t top = room.index - 3;
        int32_t bottom = room.index + 3;

        if (left >= 0 && left < 9 && room.index % 3 > 0) {
            auto iter = std::find_if(m_rooms.begin(), m_rooms.end(), [left](const auto& e) -> bool {
                return e.index == left;
            });
            if (iter != m_rooms.end()) {
                auto& neighbor = *iter;

                int32_t startX = room.center.x() - (room.size.x() / 2);
                int32_t endX = neighbor.center.x() + (neighbor.size.x() / 2);
                int32_t minX = Mathf::min(startX, endX);
                int32_t maxX = Mathf::max(startX, endX);

                for (int32_t x = minX; x <= maxX; x++) {
                    for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                        table[x][y][neighbor.center.z() - 2] = 1;
                        table[x][y][neighbor.center.z() + 2] = 1;
                    }
                    table[x][0][neighbor.center.z() - 1] = 1;
                    table[x][0][neighbor.center.z()] = 1;
                    table[x][0][neighbor.center.z() + 1] = 1;
                }
            }
        }
        if (right >= 0 && right < 9 && room.index % 3 == 1) {
            auto iter = std::find_if(m_rooms.begin(), m_rooms.end(), [right](const auto& e) -> bool {
                return e.index == right;
            });
            if (iter != m_rooms.end()) {
                auto& neighbor = *iter;

                int32_t startX = room.center.x() + (room.size.x() / 2);
                int32_t endX = neighbor.center.x() - (neighbor.size.x() / 2);
                int32_t minX = Mathf::min(startX, endX);
                int32_t maxX = Mathf::max(startX, endX);

                for (int32_t x = minX; x <= maxX; x++) {
                    for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                        table[x][y][neighbor.center.z() - 2] = 1;
                        table[x][y][neighbor.center.z() + 2] = 1;
                    }
                    table[x][0][neighbor.center.z() - 1] = 1;
                    table[x][0][neighbor.center.z()] = 1;
                    table[x][0][neighbor.center.z() + 1] = 1;
                }
            }
        }
        if (top >= 0 && top < 9 && room.index >= 3) {
            auto iter = std::find_if(m_rooms.begin(), m_rooms.end(), [top](const auto& e) -> bool {
                return e.index == top;
            });
            if (iter != m_rooms.end()) {
                auto& neighbor = *iter;

                int32_t startZ = room.center.z() - (room.size.z() / 2);
                int32_t endZ = neighbor.center.z() + (neighbor.size.z() / 2);
                int32_t minZ = Mathf::min(startZ, endZ);
                int32_t maxZ = Mathf::max(startZ, endZ);

                for (int32_t z = minZ; z <= maxZ; z++) {
                    for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                        table[neighbor.center.x() - 2][y][z] = 1;
                        table[neighbor.center.x() + 2][y][z] = 1;
                    }
                    table[neighbor.center.x() - 1][0][z] = 1;
                    table[neighbor.center.x()][0][z] = 1;
                    table[neighbor.center.x() + 1][0][z] = 1;
                }
            }
        }
        if (bottom >= 0 && bottom < 9 && room.index <= 5) {
            auto iter = std::find_if(m_rooms.begin(), m_rooms.end(), [bottom](const auto& e) -> bool {
                return e.index == bottom;
            });
            if (iter != m_rooms.end()) {
                auto& neighbor = *iter;

                int32_t startZ = room.center.z() + (room.size.z() / 2);
                int32_t endZ = neighbor.center.z() - (neighbor.size.z() / 2);
                int32_t minZ = Mathf::min(startZ, endZ);
                int32_t maxZ = Mathf::max(startZ, endZ);

                for (int32_t z = minZ; z <= maxZ; z++) {
                    for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                        table[neighbor.center.x() - 2][y][z] = 1;
                        table[neighbor.center.x() + 2][y][z] = 1;
                    }
                    table[neighbor.center.x() - 1][0][z] = 1;
                    table[neighbor.center.x()][0][z] = 1;
                    table[neighbor.center.x() + 1][0][z] = 1;
                }
            }
        }
    }

    // 孤島を見つける
    std::vector<std::vector<int32_t>> groups;
    for (const auto& room : m_rooms) {
        std::vector<int32_t> markers = markRecursive(room.index, m_rooms);

        int32_t count = 0;
        for (int32_t m : markers) {
            for (const auto& group : groups) {
                for (int32_t t : group) {
                    if (t == m) {
                        count++;
                    }
                }
            }
        }
        if (count == 0) {
            groups.emplace_back(markers);
        }
    }
    for (auto& group : groups) {
        auto iter = std::remove_if(group.begin(), group.end(), [&](const auto& e) -> bool {
            auto iter = std::find_if(m_rooms.begin(), m_rooms.end(), [e](const auto& r) -> bool {
                return r.index == e;
            });
            return iter == m_rooms.end();
        });
        group.erase(iter, group.end());
    }

    auto groupIter = std::remove_if(groups.begin(), groups.end(), [](const auto& e) -> bool {
        return e.empty();
    });
    groups.erase(groupIter, groups.end());

    // 孤島をつなぐ
    for (int32_t i = 0; i < static_cast<int32_t>(groups.size()); i++) {
        if (i + 1 >= static_cast<int32_t>(groups.size())) {
            break;
        }
        int32_t srcIndex = groups.at(i).at(rand.range(0, static_cast<int32_t>(groups.at(i).size() - 1)));
        int32_t dstIndex = groups.at(i + 1).at(rand.range(0, static_cast<int32_t>(groups.at(i + 1).size() - 1)));

        auto& src = *std::find_if(m_rooms.begin(), m_rooms.end(), [srcIndex](const auto& e) -> bool {
            return e.index == srcIndex;
        });
        auto& dst = *std::find_if(m_rooms.begin(), m_rooms.end(), [dstIndex](const auto& e) -> bool {
            return e.index == dstIndex;
        });

        int32_t startX = src.center.x();
        int32_t startZ = src.center.z();
        int32_t endX = dst.center.x();
        int32_t endZ = dst.center.z();

        // X方向に廊下を生成
        int32_t x = startX;
        while (x != endX && x >= 0 && x < k_sizeX) {
            table[x][0][startZ] = 1;
            if (startZ - 1 >= 0) {
                table[x][0][startZ - 1] = 1;
            }
            if (startZ + 1 < k_sizeZ) {
                table[x][0][startZ + 1] = 1;
            }
            for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                if (startZ - 1 >= 0) {
                    table[x][y][startZ - 1] = 1;
                }
                if (startZ + 1 < k_sizeZ) {
                    table[x][y][startZ + 1] = 1;
                }
            }
            x += (x < endX) ? 1 : -1;
        }

        // Z方向に廊下を生成
        int32_t z = startZ;
        while (z != endZ && z >= 0 && z < k_sizeZ) {
            table[endX][0][z] = 1;
            if (endX - 1 >= 0) {
                table[endX - 1][0][z] = 1;
            }
            if (endX + 1 < k_sizeX) {
                table[endX + 1][0][z] = 1;
            }
            for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                if (endX - 1 >= 0) {
                    table[endX - 1][y][z] = 1;
                }
                if (endX + 1 < k_sizeX) {
                    table[endX + 1][y][z] = 1;
                }
            }
            z += (z < endZ) ? 1 : -1;
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
const std::vector<FieldGenerator::Room>& FieldGenerator::getRooms() const { return m_rooms; }

// private
void FieldGenerator::markRecursive(int32_t index, const std::vector<Room>& rooms, std::vector<int32_t>& visit)
{
    if (std::find(visit.begin(), visit.end(), index) != visit.end()) {
        return;
    }
    visit.emplace_back(index);

    // 通路を繋ぐ
    auto& room = *std::find_if(rooms.begin(), rooms.end(), [index](const auto& e) -> bool {
        return e.index == index;
    });
    int32_t left = room.index - 1;
    int32_t right = room.index + 1;
    int32_t top = room.index - 3;
    int32_t bottom = room.index + 3;

    if (left >= 0 && left < 9 && room.index % 3 > 0) {
        auto iter = std::find_if(rooms.begin(), rooms.end(), [left](const auto& e) -> bool {
            return e.index == left;
        });
        if (iter != rooms.end()) {
            markRecursive(left, rooms, visit);
        }
    }
    if (right >= 0 && right < 9 && room.index % 3 == 1) {
        auto iter = std::find_if(rooms.begin(), rooms.end(), [right](const auto& e) -> bool {
            return e.index == right;
        });
        if (iter != rooms.end()) {
            markRecursive(right, rooms, visit);
        }
    }
    if (top >= 0 && top < 9 && room.index >= 3) {
        auto iter = std::find_if(rooms.begin(), rooms.end(), [top](const auto& e) -> bool {
            return e.index == top;
        });
        if (iter != rooms.end()) {
            markRecursive(top, rooms, visit);
        }
    }
    if (bottom >= 0 && bottom < 9 && room.index <= 5) {
        auto iter = std::find_if(rooms.begin(), rooms.end(), [bottom](const auto& e) -> bool {
            return e.index == bottom;
        });
        if (iter != rooms.end()) {
            markRecursive(bottom, rooms, visit);
        }
    }
}

std::vector<int32_t> FieldGenerator::markRecursive(int32_t index, const std::vector<Room>& rooms)
{
    std::vector<int32_t> temp;
    markRecursive(index, rooms, temp);
    return temp;
}
}