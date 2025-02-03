#include <Scenes/Game/System/Chunk.hpp>
#include <Scenes/Game/System/ChunkGenerator.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace App::Scenes::Game::System {
// Room
ChunkGenerator::Room::Room()
    : size()
    , center()
    , index()
    , removed()
    , connected()
{
}
// Route
ChunkGenerator::Route::Route()
    : prevRoomIndex()
    , nextRoomIndex()
    , roomIndex()
    , center()
{
}
// public
ChunkGenerator::ChunkGenerator() { }
void ChunkGenerator::generate()
{
    const int32_t k_sizeX = Chunk::k_chunkSizeX;
    const int32_t k_sizeY = Chunk::k_chunkSizeY;
    const int32_t k_sizeZ = Chunk::k_chunkSizeZ;

    const int32_t k_blockNone = 0;
    const int32_t k_blockRoomFloor = 1;
    const int32_t k_blockRoomRoof = 2;
    const int32_t k_blockRoomWall = 3;
    const int32_t k_blockRouteFloor = 4;
    const int32_t k_blockRouteRoof = 5;
    const int32_t k_blockRouteWall = 6;

    Random rand;
    const int32_t k_space = Chunk::k_roomSpace;
    int32_t roomSizeX = Chunk::k_roomSizeX;
    int32_t roomSizeZ = Chunk::k_roomSizeZ;

    m_rooms.clear();
    m_routes.clear();

    // 空間を9分割
    int32_t roomIndex = 0;
    for (int32_t j = 0; j < 3; j++) {
        for (int32_t i = 0; i < 3; i++) {
            int32_t centerX = (i * k_space) + (roomSizeX * i) + (roomSizeX / 2);
            int32_t centerZ = (j * k_space) + (roomSizeZ * j) + (roomSizeZ / 2);

            Room room;
            room.index = roomIndex;
            room.center = IntVector3({ centerX, 0, centerZ });
            room.size = IntVector3({ roomSizeX, Chunk::k_chunkSizeY, roomSizeZ });
            m_rooms.emplace_back(room);
            roomIndex++;
        }
    }

    // ブロック座標を記憶する配列を確保
    // スタックを使いすぎないように動的確保
    std::vector<std::vector<std::vector<int32_t>>> table(Chunk::k_chunkSizeX);
    for (int32_t x = 0; x < k_sizeX; x++) {
        std::vector<std::vector<int32_t>> inner(Chunk::k_chunkSizeY);
        for (int32_t y = 0; y < k_sizeY; y++) {
            std::vector<int32_t> vec(Chunk::k_chunkSizeZ);
            inner.at(y) = std::move(vec);
        }
        table.at(x) = std::move(inner);
    }

    // ルームの床にブロックを配置
    for (const auto& room : m_rooms) {
        for (int32_t x = 0; x < room.size.x(); x++) {
            for (int32_t z = 0; z < room.size.z(); z++) {
                int32_t tileX = (room.center.x() - (room.size.x() / 2)) + x;
                int32_t tileZ = (room.center.z() - (room.size.z() / 2)) + z;
                table[tileX][0][tileZ] = k_blockRoomFloor;
                table[tileX][Chunk::k_chunkSizeY - 1][tileZ] = k_blockRoomRoof;
            }
        }
        for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
            int32_t minX = (room.center.x() - (room.size.x() / 2));
            int32_t maxX = (room.center.x() + (room.size.x() / 2));
            int32_t minZ = (room.center.z() - (room.size.z() / 2));
            int32_t maxZ = (room.center.z() + (room.size.z() / 2));

            for (int32_t x = 0; x < room.size.x(); x++) {
                int32_t tileX = (room.center.x() - (room.size.x() / 2)) + x;
                table[tileX][y][minZ] = k_blockRoomWall;
                table[tileX][y][maxZ] = k_blockRoomWall;
            }
            for (int32_t z = 0; z < room.size.z(); z++) {
                int32_t tileZ = (room.center.z() - (room.size.z() / 2)) + z;
                table[minX][y][tileZ] = k_blockRoomWall;
                table[maxX][y][tileZ] = k_blockRoomWall;
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
                    for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
                        table[x][y][neighbor.center.z() - 2] = k_blockRouteWall;
                        table[x][y][neighbor.center.z() + 2] = k_blockRouteWall;
                    }
                    table[x][0][neighbor.center.z() - 1] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z()] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z() + 1] = k_blockRouteFloor;
                    table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z() - 1] = k_blockRouteRoof;
                    table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z()] = k_blockRouteRoof;
                    table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z() + 1] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                    table[minX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[minX][y][neighbor.center.z()] = k_blockNone;
                    table[minX][y][neighbor.center.z() + 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z()] = k_blockNone;
                    table[maxX][y][neighbor.center.z() + 1] = k_blockNone;
                }
            }
        }
        if (room.index % 3 == 0) {
            int32_t leftX = room.center.x() - ((room.size.x() / 2));
            int32_t startZ = room.center.z() - 1;
            int32_t endZ = room.center.z() + 1;
            int32_t minZ = Mathf::min(startZ, endZ);
            int32_t maxZ = Mathf::max(startZ, endZ);
            for (int32_t z = startZ; z <= endZ; z++) {
                for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                    table[leftX][y][z] = k_blockNone;
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
                    for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
                        table[x][y][neighbor.center.z() - 2] = k_blockRouteWall;
                        table[x][y][neighbor.center.z() + 2] = k_blockRouteWall;
                    }
                    table[x][0][neighbor.center.z() - 1] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z()] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z() + 1] = k_blockRouteFloor;
                    table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z() - 1] = k_blockRouteRoof;
                    table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z()] = k_blockRouteRoof;
                    table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z() + 1] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                    table[minX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[minX][y][neighbor.center.z()] = k_blockNone;
                    table[minX][y][neighbor.center.z() + 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z()] = k_blockNone;
                    table[maxX][y][neighbor.center.z() + 1] = k_blockNone;
                }
            }
        }
        if (room.index % 3 == 2) {
            Room neighbor;
            neighbor.center = room.center + IntVector3({ Chunk::k_roomSpace, 0, 0 }) + IntVector3({ Chunk::k_roomSizeX, 0, 0 });
            neighbor.size = IntVector3({ roomSizeX, Chunk::k_chunkSizeY, roomSizeZ });

            int32_t startX = room.center.x() + (room.size.x() / 2);
            int32_t endX = neighbor.center.x() - (neighbor.size.x() / 2) - 1;
            int32_t minX = Mathf::min(startX, endX);
            int32_t maxX = Mathf::max(startX, endX);

            for (int32_t x = minX; x <= maxX; x++) {
                for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
                    table[x][y][neighbor.center.z() - 2] = k_blockRouteWall;
                    table[x][y][neighbor.center.z() + 2] = k_blockRouteWall;
                }
                table[x][0][neighbor.center.z() - 1] = k_blockRouteFloor;
                table[x][0][neighbor.center.z()] = k_blockRouteFloor;
                table[x][0][neighbor.center.z() + 1] = k_blockRouteFloor;
                table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z() - 1] = k_blockRouteRoof;
                table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z()] = k_blockRouteRoof;
                table[x][Chunk::k_chunkSizeY - 1][neighbor.center.z() + 1] = k_blockRouteRoof;
            }
            for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                table[minX][y][neighbor.center.z() - 1] = k_blockNone;
                table[minX][y][neighbor.center.z()] = k_blockNone;
                table[minX][y][neighbor.center.z() + 1] = k_blockNone;
                table[maxX][y][neighbor.center.z() - 1] = k_blockNone;
                table[maxX][y][neighbor.center.z()] = k_blockNone;
                table[maxX][y][neighbor.center.z() + 1] = k_blockNone;
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
                    for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
                        table[neighbor.center.x() - 2][y][z] = k_blockRouteWall;
                        table[neighbor.center.x() + 2][y][z] = k_blockRouteWall;
                    }
                    table[neighbor.center.x() - 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x()][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() + 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() - 1][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x()][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x() + 1][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                    table[neighbor.center.x() - 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x()][y][minZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x() - 1][y][maxZ] = k_blockNone;
                    table[neighbor.center.x()][y][maxZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][maxZ] = k_blockNone;
                }
            }
        }
        if (room.index < 3) {
            int32_t nearZ = room.center.z() - ((room.size.z() / 2));
            int32_t startX = room.center.x() - 1;
            int32_t endX = room.center.x() + 1;
            int32_t minX = Mathf::min(startX, endX);
            int32_t maxX = Mathf::max(startX, endX);
            for (int32_t x = startX; x <= endX; x++) {
                for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                    table[x][y][nearZ] = k_blockNone;
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
                    for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
                        table[neighbor.center.x() - 2][y][z] = k_blockRouteWall;
                        table[neighbor.center.x() + 2][y][z] = k_blockRouteWall;
                    }
                    table[neighbor.center.x() - 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x()][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() + 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() - 1][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x()][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x() + 1][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                    table[neighbor.center.x() - 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x()][y][minZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x() - 1][y][maxZ] = k_blockNone;
                    table[neighbor.center.x()][y][maxZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][maxZ] = k_blockNone;
                }
            }
        }
        if (room.index > 5) {
            Room neighbor;
            neighbor.center = room.center + IntVector3({ 0, 0, Chunk::k_roomSpace }) + IntVector3({ 0, 0, Chunk::k_roomSizeZ });
            neighbor.size = IntVector3({ roomSizeX, Chunk::k_chunkSizeY, roomSizeZ });

            int32_t startZ = room.center.z() + (room.size.z() / 2);
            int32_t endZ = neighbor.center.z() - (neighbor.size.z() / 2) - 1;
            int32_t minZ = Mathf::min(startZ, endZ);
            int32_t maxZ = Mathf::max(startZ, endZ);

            for (int32_t z = minZ; z <= maxZ; z++) {
                for (int32_t y = 0; y < Chunk::k_chunkSizeY; y++) {
                    table[neighbor.center.x() - 2][y][z] = k_blockRouteWall;
                    table[neighbor.center.x() + 2][y][z] = k_blockRouteWall;
                }
                table[neighbor.center.x() - 1][0][z] = k_blockRouteFloor;
                table[neighbor.center.x()][0][z] = k_blockRouteFloor;
                table[neighbor.center.x() + 1][0][z] = k_blockRouteFloor;
                table[neighbor.center.x() - 1][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                table[neighbor.center.x()][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
                table[neighbor.center.x() + 1][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
            }
            for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
                table[neighbor.center.x() - 1][y][minZ] = k_blockNone;
                table[neighbor.center.x()][y][minZ] = k_blockNone;
                table[neighbor.center.x() + 1][y][minZ] = k_blockNone;
                table[neighbor.center.x() - 1][y][maxZ] = k_blockNone;
                table[neighbor.center.x()][y][maxZ] = k_blockNone;
                table[neighbor.center.x() + 1][y][maxZ] = k_blockNone;
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
                float fx = static_cast<float>(x);
                float fy = static_cast<float>(y);
                float fz = static_cast<float>(z);
                for (int32_t normal = 0; normal < Chunk::k_normalVectorTable.size(); normal++) {
                    float fNormal = static_cast<float>(normal);
                    const auto& normalVec = Chunk::k_normalVectorTable.at(normal);
                    const IntVector3 neighbor = IntVector3({ x, y, z }) + (IntVector3)normalVec;

                    bool outX = neighbor.x() < 0 || neighbor.x() >= Chunk::k_chunkSizeX;
                    bool outY = neighbor.y() < 0 || neighbor.y() >= Chunk::k_chunkSizeY;
                    bool outZ = neighbor.z() < 0 || neighbor.z() >= Chunk::k_chunkSizeZ;

                    if (outX || outY || outZ) {
                        continue;
                    }

                    if (!table[neighbor.x()][neighbor.y()][neighbor.z()]) {
                        m_tiles.emplace_back(Vector4({ fx, fy, fz, fNormal }));
                    }
                }
            }
        }
    }
}
const std::vector<Vector4>& ChunkGenerator::getTiles() const { return m_tiles; }
const std::vector<ChunkGenerator::Room>& ChunkGenerator::getRooms() const { return m_rooms; }

// private
void ChunkGenerator::markRecursive(int32_t index, const std::vector<Room>& rooms, std::vector<int32_t>& visit)
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
    if (right >= 0 && right < 9 && room.index % 3 < 2) {
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

std::vector<int32_t> ChunkGenerator::markRecursive(int32_t index, const std::vector<Room>& rooms)
{
    std::vector<int32_t> temp;
    markRecursive(index, rooms, temp);
    return temp;
}
}