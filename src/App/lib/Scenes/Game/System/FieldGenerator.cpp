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
// Route
FieldGenerator::Route::Route()
    : prevRoomIndex()
    , nextRoomIndex()
    , roomIndex()
    , center()
{
}
// public
FieldGenerator::FieldGenerator() { }
void FieldGenerator::generate()
{
    const int32_t k_sizeX = Field::k_fieldSizeX;
    const int32_t k_sizeY = Field::k_fieldSizeY;
    const int32_t k_sizeZ = Field::k_fieldSizeZ;

    const int32_t k_blockNone = 0;
    const int32_t k_blockRoomFloor = 1;
    const int32_t k_blockRoomRoof = 2;
    const int32_t k_blockRoomWall = 3;
    const int32_t k_blockRouteFloor = 4;
    const int32_t k_blockRouteRoof = 5;
    const int32_t k_blockRouteWall = 6;

    Random rand;
    const int32_t k_space = Field::k_roomSpace;
    int32_t roomSizeX = Field::k_roomSizeX;
    int32_t roomSizeZ = Field::k_roomSizeZ;

    m_rooms.clear();
    m_routes.clear();

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
    // スタックを使いすぎないように動的確保
    std::vector<std::vector<std::vector<int32_t>>> table(Field::k_fieldSizeX);
    for (int32_t x = 0; x < k_sizeX; x++) {
        std::vector<std::vector<int32_t>> inner(Field::k_fieldSizeY);
        for (int32_t y = 0; y < k_sizeY; y++) {
            std::vector<int32_t> vec(Field::k_fieldSizeZ);
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
                table[tileX][Field::k_fieldSizeY - 1][tileZ] = k_blockRoomRoof;
            }
        }
        for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
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
                    for (int32_t y = 0; y < Field::k_fieldSizeY; y++) {
                        table[x][y][neighbor.center.z() - 2] = k_blockRouteWall;
                        table[x][y][neighbor.center.z() + 2] = k_blockRouteWall;
                    }
                    table[x][0][neighbor.center.z() - 1] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z()] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z() + 1] = k_blockRouteFloor;
                    table[x][Field::k_fieldSizeY - 1][neighbor.center.z() - 1] = k_blockRouteRoof;
                    table[x][Field::k_fieldSizeY - 1][neighbor.center.z()] = k_blockRouteRoof;
                    table[x][Field::k_fieldSizeY - 1][neighbor.center.z() + 1] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                    table[minX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[minX][y][neighbor.center.z()] = k_blockNone;
                    table[minX][y][neighbor.center.z() + 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z()] = k_blockNone;
                    table[maxX][y][neighbor.center.z() + 1] = k_blockNone;
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
                        table[x][y][neighbor.center.z() - 2] = k_blockRouteWall;
                        table[x][y][neighbor.center.z() + 2] = k_blockRouteWall;
                    }
                    table[x][0][neighbor.center.z() - 1] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z()] = k_blockRouteFloor;
                    table[x][0][neighbor.center.z() + 1] = k_blockRouteFloor;
                    table[x][Field::k_fieldSizeY - 1][neighbor.center.z() - 1] = k_blockRouteRoof;
                    table[x][Field::k_fieldSizeY - 1][neighbor.center.z()] = k_blockRouteRoof;
                    table[x][Field::k_fieldSizeY - 1][neighbor.center.z() + 1] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                    table[minX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[minX][y][neighbor.center.z()] = k_blockNone;
                    table[minX][y][neighbor.center.z() + 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z() - 1] = k_blockNone;
                    table[maxX][y][neighbor.center.z()] = k_blockNone;
                    table[maxX][y][neighbor.center.z() + 1] = k_blockNone;
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
                        table[neighbor.center.x() - 2][y][z] = k_blockRouteWall;
                        table[neighbor.center.x() + 2][y][z] = k_blockRouteWall;
                    }
                    table[neighbor.center.x() - 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x()][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() + 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                    table[neighbor.center.x() - 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x()][y][minZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x() - 1][y][maxZ] = k_blockNone;
                    table[neighbor.center.x()][y][maxZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][maxZ] = k_blockNone;
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
                        table[neighbor.center.x() - 2][y][z] = k_blockRouteWall;
                        table[neighbor.center.x() + 2][y][z] = k_blockRouteWall;
                    }
                    table[neighbor.center.x() - 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x()][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() + 1][0][z] = k_blockRouteFloor;
                    table[neighbor.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    table[neighbor.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                }
                for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                    table[neighbor.center.x() - 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x()][y][minZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][minZ] = k_blockNone;
                    table[neighbor.center.x() - 1][y][maxZ] = k_blockNone;
                    table[neighbor.center.x()][y][maxZ] = k_blockNone;
                    table[neighbor.center.x() + 1][y][maxZ] = k_blockNone;
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
        // 孤島グループからランダムに一組の部屋を選ぶ
        std::vector<int32_t> srcGroup = groups.at(i);
        std::vector<int32_t> dstGroup = groups.at(i + 1);
        int32_t srcIndex = srcGroup.at(rand.range(0, static_cast<int32_t>(srcGroup.size() - 1)));
        int32_t dstIndex = dstGroup.at(rand.range(0, static_cast<int32_t>(dstGroup.size() - 1)));

        Room& srcRoom = *std::find_if(m_rooms.begin(), m_rooms.end(), [srcIndex](const auto& r) -> bool {
            return r.index == srcIndex;
        });
        Room& dstRoom = *std::find_if(m_rooms.begin(), m_rooms.end(), [dstIndex](const auto& r) -> bool {
            return r.index == dstIndex;
        });
        int32_t start = srcRoom.index;
        std::vector<std::vector<int32_t>> paths;
        // 部屋と部屋を繋ぐパスを作る
        // 途中で部屋にぶつかるたびに新たなパスとして追加する
        while (1) {
            std::vector<int32_t> path;
            int32_t current = start;
            while (1) {
                path.emplace_back(current);

                if (current != start) {
                    auto iter = std::find_if(m_rooms.begin(), m_rooms.end(), [current](const auto& r) -> bool {
                        return r.index == current;
                    });
                    if (iter != m_rooms.end()) {
                        break;
                    }
                }

                if (current == dstRoom.index) {
                    break;
                }

                int32_t srcRow = current / 3;
                int32_t dstRow = dstRoom.index / 3;
                int32_t srcColumn = current % 3;
                int32_t dstColumn = dstRoom.index % 3;
                int32_t branch = rand.range(0, 1);
                if (branch == 0) {
                    if (srcRow != dstRow) {
                        if (srcRow < dstRow) {
                            current += 3;
                        } else {
                            current -= 3;
                        }
                    } else if (srcColumn != dstColumn) {
                        if (srcColumn < dstColumn) {
                            current++;
                        } else {
                            current--;
                        }
                    }
                } else {
                    if (srcColumn != dstColumn) {
                        if (srcColumn < dstColumn) {
                            current++;
                        } else {
                            current--;
                        }
                    } else if (srcRow != dstRow) {
                        if (srcRow < dstRow) {
                            current += 3;
                        } else {
                            current -= 3;
                        }
                    }
                }
            }
            paths.emplace_back(path);

            start = current;
            if (current == dstRoom.index) {
                break;
            }
        }

        // 孤島の内側に閉じたパスは不要なので削除
        auto iter = std::remove_if(paths.begin(), paths.end(), [](const auto& p) -> bool {
            return p.size() == 2;
        });
        paths.erase(iter, paths.end());

        // パスにそって通路を繋ぐ
        for (const auto& path : paths) {
            auto pathIter = path.begin();
            int32_t current = *pathIter;
            ++pathIter;

            int32_t end = path.at(path.size() - 1);

            while (current != end) {
                int32_t next = end;
                if (pathIter != path.end()) {
                    next = *pathIter;
                    ++pathIter;
                }

                int32_t currentRow = current / 3;
                int32_t currentCol = current % 3;

                int32_t nextRow = next / 3;
                int32_t nextCol = next % 3;

                Room currentRoom;
                currentRoom.index = current;
                currentRoom.center = IntVector3({ ((currentCol + 1) * k_space) + (roomSizeX * currentCol) + (roomSizeX / 2), 0, ((currentRow + 1) * k_space) + (roomSizeZ * currentRow) + (roomSizeZ / 2) });
                currentRoom.size = IntVector3({ roomSizeX, Field::k_fieldSizeY, roomSizeZ });

                Room nextRoom;
                nextRoom.index = next;
                nextRoom.center = IntVector3({ ((nextCol + 1) * k_space) + (roomSizeX * nextCol) + (roomSizeX / 2), 0, ((nextRow + 1) * k_space) + (roomSizeZ * nextRow) + (roomSizeZ / 2) });
                nextRoom.size = IntVector3({ roomSizeX, Field::k_fieldSizeY, roomSizeZ });

                if (pathIter != path.end()) {
                    Route route;
                    route.prevRoomIndex = current;
                    route.nextRoomIndex = *pathIter;
                    route.roomIndex = next;
                    route.center = nextRoom.center;
                    m_routes.emplace_back(route);
                }

                if (currentRow != nextRow) {
                    if (currentRow > nextRow) {
                        int32_t startZ = currentRoom.center.z() - (currentRoom.size.z() / 2);
                        int32_t endZ = nextRoom.center.z() + (nextRoom.size.z() / 2);

                        int32_t minZ = Mathf::min(startZ, endZ);
                        int32_t maxZ = Mathf::max(startZ, endZ);

                        for (int32_t z = minZ; z <= maxZ; z++) {
                            table[currentRoom.center.x() - 1][0][z] = k_blockRouteFloor;
                            table[currentRoom.center.x()][0][z] = k_blockRouteFloor;
                            table[currentRoom.center.x() + 1][0][z] = k_blockRouteFloor;
                            table[currentRoom.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[currentRoom.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[currentRoom.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        }
                        for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                            table[currentRoom.center.x() - 1][y][minZ] = k_blockNone;
                            table[currentRoom.center.x()][y][minZ] = k_blockNone;
                            table[currentRoom.center.x() + 1][y][minZ] = k_blockNone;
                            table[currentRoom.center.x() - 1][y][maxZ] = k_blockNone;
                            table[currentRoom.center.x()][y][maxZ] = k_blockNone;
                            table[currentRoom.center.x() + 1][y][maxZ] = k_blockNone;
                        }
                    } else {
                        int32_t startZ = currentRoom.center.z() + (currentRoom.size.z() / 2);
                        int32_t endZ = nextRoom.center.z() - (nextRoom.size.z() / 2);

                        int32_t minZ = Mathf::min(startZ, endZ);
                        int32_t maxZ = Mathf::max(startZ, endZ);

                        for (int32_t z = minZ; z <= maxZ; z++) {
                            table[currentRoom.center.x() - 1][0][z] = k_blockRouteFloor;
                            table[currentRoom.center.x()][0][z] = k_blockRouteFloor;
                            table[currentRoom.center.x() + 1][0][z] = k_blockRouteFloor;
                            table[currentRoom.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[currentRoom.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[currentRoom.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        }
                        for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                            table[currentRoom.center.x() - 1][y][minZ] = k_blockNone;
                            table[currentRoom.center.x()][y][minZ] = k_blockNone;
                            table[currentRoom.center.x() + 1][y][minZ] = k_blockNone;
                            table[currentRoom.center.x() - 1][y][maxZ] = k_blockNone;
                            table[currentRoom.center.x()][y][maxZ] = k_blockNone;
                            table[currentRoom.center.x() + 1][y][maxZ] = k_blockNone;
                        }
                    }
                } else if (currentCol != nextCol) {
                    if (currentCol > nextCol) {
                        int32_t startX = currentRoom.center.x() - (currentRoom.size.x() / 2);
                        int32_t endX = nextRoom.center.x() + (nextRoom.size.x() / 2);

                        int32_t minX = Mathf::min(startX, endX);
                        int32_t maxX = Mathf::max(startX, endX);

                        for (int32_t x = minX; x <= maxX; x++) {
                            table[x][0][currentRoom.center.z() - 1] = k_blockRouteFloor;
                            table[x][0][currentRoom.center.z()] = k_blockRouteFloor;
                            table[x][0][currentRoom.center.z() + 1] = k_blockRouteFloor;
                            table[x][Field::k_fieldSizeY - 1][currentRoom.center.z() - 1] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][currentRoom.center.z()] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][currentRoom.center.z() + 1] = k_blockRouteRoof;
                        }
                        for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                            table[minX][y][currentRoom.center.z() - 1] = k_blockNone;
                            table[minX][y][currentRoom.center.z()] = k_blockNone;
                            table[minX][y][currentRoom.center.z() + 1] = k_blockNone;
                            table[maxX][y][currentRoom.center.z() - 1] = k_blockNone;
                            table[maxX][y][currentRoom.center.z()] = k_blockNone;
                            table[maxX][y][currentRoom.center.z() + 1] = k_blockNone;
                        }
                    } else {
                        int32_t startX = currentRoom.center.x() + (currentRoom.size.x() / 2);
                        int32_t endX = nextRoom.center.x() - (nextRoom.size.x() / 2);

                        int32_t minX = Mathf::min(startX, endX);
                        int32_t maxX = Mathf::max(startX, endX);

                        for (int32_t x = minX; x <= maxX; x++) {
                            table[x][0][currentRoom.center.z() - 1] = k_blockRouteFloor;
                            table[x][0][currentRoom.center.z()] = k_blockRouteFloor;
                            table[x][0][currentRoom.center.z() + 1] = k_blockRouteFloor;
                            table[x][Field::k_fieldSizeY - 1][currentRoom.center.z() - 1] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][currentRoom.center.z()] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][currentRoom.center.z() + 1] = k_blockRouteRoof;
                        }
                        for (int32_t y = 1; y < Field::k_fieldSizeY - 1; y++) {
                            table[minX][y][currentRoom.center.z() - 1] = k_blockNone;
                            table[minX][y][currentRoom.center.z()] = k_blockNone;
                            table[minX][y][currentRoom.center.z() + 1] = k_blockNone;
                            table[maxX][y][currentRoom.center.z() - 1] = k_blockNone;
                            table[maxX][y][currentRoom.center.z()] = k_blockNone;
                            table[maxX][y][currentRoom.center.z() + 1] = k_blockNone;
                        }
                    }
                }
                current = next;
            }
        }
    }

    // 通路を直線またはL字で埋める
    for (int32_t routeIndex = 0; routeIndex < m_routes.size(); routeIndex++) {
        Route& route = m_routes.at(routeIndex);

        int32_t fromRow = route.prevRoomIndex / 3;
        int32_t fromCol = route.prevRoomIndex % 3;

        int32_t midRow = route.roomIndex / 3;
        int32_t midCol = route.roomIndex % 3;

        int32_t toRow = route.nextRoomIndex / 3;
        int32_t toCol = route.nextRoomIndex % 3;

        if (fromRow == midRow) {
            if (midRow == toRow) {
                int32_t startX = route.center.x() - (roomSizeX / 2);
                int32_t endX = route.center.x() + (roomSizeX / 2);

                int32_t minX = Mathf::min(startX, endX);
                int32_t maxX = Mathf::max(startX, endX);

                for (int32_t x = minX; x < maxX; x++) {
                    table[x][0][route.center.z() - 1] = k_blockRouteFloor;
                    table[x][0][route.center.z()] = k_blockRouteFloor;
                    table[x][0][route.center.z() + 1] = k_blockRouteFloor;
                    table[x][Field::k_fieldSizeY - 1][route.center.z() - 1] = k_blockRouteRoof;
                    table[x][Field::k_fieldSizeY - 1][route.center.z()] = k_blockRouteRoof;
                    table[x][Field::k_fieldSizeY - 1][route.center.z() + 1] = k_blockRouteRoof;
                }
            } else {
                if (midRow > toRow) {
                    int32_t endX = route.center.x();
                    if (fromCol > midCol) {
                        int32_t startX = route.center.x() + (roomSizeX / 2);

                        int32_t minX = Mathf::min(startX, endX);
                        int32_t maxX = Mathf::max(startX, endX);

                        for (int32_t x = minX; x <= maxX; x++) {
                            table[x][0][route.center.z() - 1] = k_blockRouteFloor;
                            table[x][0][route.center.z()] = k_blockRouteFloor;
                            table[x][0][route.center.z() + 1] = k_blockRouteFloor;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() - 1] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z()] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() + 1] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    } else if (fromCol < midCol) {
                        int32_t startX = route.center.x() - (roomSizeX / 2);
                        int32_t endX = route.center.x();

                        int32_t minX = Mathf::min(startX, endX);
                        int32_t maxX = Mathf::max(startX, endX);

                        for (int32_t x = minX; x <= maxX; x++) {
                            table[x][0][route.center.z() - 1] = k_blockRouteFloor;
                            table[x][0][route.center.z()] = k_blockRouteFloor;
                            table[x][0][route.center.z() + 1] = k_blockRouteFloor;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() - 1] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z()] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() + 1] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    }

                    int32_t startZ = route.center.z();
                    int32_t endZ = route.center.z() - (roomSizeZ / 2);

                    int32_t minZ = Mathf::min(startZ, endZ);
                    int32_t maxZ = Mathf::max(startZ, endZ);
                    for (int32_t z = minZ; z <= maxZ; z++) {
                        table[endX - 1][0][z] = k_blockRouteFloor;
                        table[endX][0][z] = k_blockRouteFloor;
                        table[endX + 1][0][z] = k_blockRouteFloor;
                        table[endX - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        table[endX][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        table[endX + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    }
                } else if (midRow < toRow) {
                    int32_t endX = route.center.x();
                    if (fromCol > midCol) {
                        int32_t startX = route.center.x() + (roomSizeX / 2);

                        int32_t minX = Mathf::min(startX, endX);
                        int32_t maxX = Mathf::max(startX, endX);

                        for (int32_t x = minX; x <= maxX; x++) {
                            table[x][0][route.center.z() - 1] = k_blockRouteFloor;
                            table[x][0][route.center.z()] = k_blockRouteFloor;
                            table[x][0][route.center.z() + 1] = k_blockRouteFloor;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() - 1] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z()] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() + 1] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    } else if (fromCol < midCol) {
                        int32_t startX = route.center.x() - (roomSizeX / 2);
                        int32_t endX = route.center.x();

                        int32_t minX = Mathf::min(startX, endX);
                        int32_t maxX = Mathf::max(startX, endX);

                        for (int32_t x = minX; x <= maxX; x++) {
                            table[x][0][route.center.z() - 1] = k_blockRouteFloor;
                            table[x][0][route.center.z()] = k_blockRouteFloor;
                            table[x][0][route.center.z() + 1] = k_blockRouteFloor;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() - 1] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z()] = k_blockRouteRoof;
                            table[x][Field::k_fieldSizeY - 1][route.center.z() + 1] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    }

                    int32_t startZ = route.center.z();
                    int32_t endZ = route.center.z() + (roomSizeZ / 2);

                    int32_t minZ = Mathf::min(startZ, endZ);
                    int32_t maxZ = Mathf::max(startZ, endZ);
                    for (int32_t z = minZ; z <= maxZ; z++) {
                        table[endX - 1][0][z] = k_blockRouteFloor;
                        table[endX][0][z] = k_blockRouteFloor;
                        table[endX + 1][0][z] = k_blockRouteFloor;
                        table[endX - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        table[endX][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        table[endX + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    }
                }
            }

        } else if (fromCol == midCol) {
            if (midCol == toCol) {
                int32_t startZ = route.center.z() - (roomSizeZ / 2);
                int32_t endZ = route.center.z() + (roomSizeZ / 2);

                int32_t minZ = Mathf::min(startZ, endZ);
                int32_t maxZ = Mathf::max(startZ, endZ);

                for (int32_t z = minZ; z < maxZ; z++) {
                    table[route.center.x() - 1][0][z] = k_blockRouteFloor;
                    table[route.center.x()][0][z] = k_blockRouteFloor;
                    table[route.center.x() + 1][0][z] = k_blockRouteFloor;
                    table[route.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    table[route.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                    table[route.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                }
            } else {
                if (midCol > toCol) {
                    int32_t endZ = route.center.z();
                    if (fromRow > midRow) {
                        int32_t startZ = route.center.z() + (roomSizeZ / 2);

                        int32_t minZ = Mathf::min(startZ, endZ);
                        int32_t maxZ = Mathf::max(startZ, endZ);

                        for (int32_t z = minZ; z <= maxZ; z++) {
                            table[route.center.x() - 1][0][z] = k_blockRouteFloor;
                            table[route.center.x()][0][z] = k_blockRouteFloor;
                            table[route.center.x() + 1][0][z] = k_blockRouteFloor;
                            table[route.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    } else if (fromRow < midRow) {
                        int32_t startZ = route.center.z() - (roomSizeZ / 2);
                        int32_t endZ = route.center.z();

                        int32_t minZ = Mathf::min(startZ, endZ);
                        int32_t maxZ = Mathf::max(startZ, endZ);

                        for (int32_t z = minZ; z <= maxZ; z++) {
                            table[route.center.x() - 1][0][z] = k_blockRouteFloor;
                            table[route.center.x()][0][z] = k_blockRouteFloor;
                            table[route.center.x() + 1][0][z] = k_blockRouteFloor;
                            table[route.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    }

                    int32_t startX = route.center.x();
                    int32_t endX = route.center.x() - (roomSizeX / 2);

                    int32_t minX = Mathf::min(startX, endX);
                    int32_t maxX = Mathf::max(startX, endX);
                    for (int32_t x = minX; x <= maxX; x++) {
                        table[x][0][endZ - 1] = k_blockRouteFloor;
                        table[x][0][endZ] = k_blockRouteFloor;
                        table[x][0][endZ + 1] = k_blockRouteFloor;
                        table[x][Field::k_fieldSizeY - 1][endZ - 1] = k_blockRouteRoof;
                        table[x][Field::k_fieldSizeY - 1][endZ] = k_blockRouteRoof;
                        table[x][Field::k_fieldSizeY - 1][endZ + 1] = k_blockRouteRoof;
                    }
                } else if (midCol < toCol) {
                    int32_t endZ = route.center.z();
                    if (fromRow > midRow) {
                        int32_t startZ = route.center.z() + (roomSizeZ / 2);

                        int32_t minZ = Mathf::min(startZ, endZ);
                        int32_t maxZ = Mathf::max(startZ, endZ);

                        for (int32_t z = minZ; z <= maxZ; z++) {
                            table[route.center.x() - 1][0][z] = k_blockRouteFloor;
                            table[route.center.x()][0][z] = k_blockRouteFloor;
                            table[route.center.x() + 1][0][z] = k_blockRouteFloor;
                            table[route.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    } else if (fromRow < midRow) {
                        int32_t startZ = route.center.z() - (roomSizeZ / 2);
                        int32_t endZ = route.center.z();

                        int32_t minZ = Mathf::min(startZ, endZ);
                        int32_t maxZ = Mathf::max(startZ, endZ);

                        for (int32_t z = minZ; z <= maxZ; z++) {
                            table[route.center.x() - 1][0][z] = k_blockRouteFloor;
                            table[route.center.x()][0][z] = k_blockRouteFloor;
                            table[route.center.x() + 1][0][z] = k_blockRouteFloor;
                            table[route.center.x() - 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x()][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                            table[route.center.x() + 1][Field::k_fieldSizeY - 1][z] = k_blockRouteRoof;
                        }
                        table[route.center.x()][0][route.center.z()] = k_blockRouteFloor;
                    }

                    int32_t startX = route.center.x();
                    int32_t endX = route.center.x() + (roomSizeX / 2);

                    int32_t minX = Mathf::min(startX, endX);
                    int32_t maxX = Mathf::max(startX, endX);
                    for (int32_t x = minX; x <= maxX; x++) {
                        table[x][0][endZ - 1] = k_blockRouteFloor;
                        table[x][0][endZ] = k_blockRouteFloor;
                        table[x][0][endZ + 1] = k_blockRouteFloor;
                        table[x][Field::k_fieldSizeY - 1][endZ - 1] = k_blockRouteRoof;
                        table[x][Field::k_fieldSizeY - 1][endZ] = k_blockRouteRoof;
                        table[x][Field::k_fieldSizeY - 1][endZ + 1] = k_blockRouteRoof;
                    }
                }
            }
        }
    }

    // 通路の壁を埋める
    for (int32_t x = 0; x < Field::k_fieldSizeX; x++) {
        const int32_t y = 0;
        for (int32_t z = 0; z < Field::k_fieldSizeZ; z++) {
            if (table[x][y][z]) {
                continue;
            }
            std::array<IntVector3, 8> neighbor {
                IntVector3({ x - 1, y, z }),
                IntVector3({ x + 1, y, z }),
                IntVector3({ x, y, z - 1 }),
                IntVector3({ x, y, z + 1 }),
                IntVector3({ x - 1, y, z - 1 }),
                IntVector3({ x + 1, y, z - 1 }),
                IntVector3({ x - 1, y, z + 1 }),
                IntVector3({ x + 1, y, z + 1 })
            };
            bool wall = false;
            for (const auto& n : neighbor) {
                if (n.x() < 0 || n.x() >= Field::k_fieldSizeX) {
                    continue;
                }
                if (n.y() < 0 || n.y() >= Field::k_fieldSizeY) {
                    continue;
                }
                if (n.z() < 0 || n.z() >= Field::k_fieldSizeZ) {
                    continue;
                }
                if (table[n.x()][n.y()][n.z()] == k_blockRouteFloor) {
                    wall = true;
                    break;
                }
            }
            if (wall) {
                for (int32_t yy = 0; yy < Field::k_fieldSizeY; yy++) {
                    table[x][yy][z] = k_blockRouteWall;
                }
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

std::vector<int32_t> FieldGenerator::markRecursive(int32_t index, const std::vector<Room>& rooms)
{
    std::vector<int32_t> temp;
    markRecursive(index, rooms, temp);
    return temp;
}
}