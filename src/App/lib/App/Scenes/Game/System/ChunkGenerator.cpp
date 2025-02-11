#include <App/Scenes/Game/System/Chunk.hpp>
#include <App/Scenes/Game/System/ChunkGenerator.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace App::Scenes::Game::System {
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

    (void)k_blockNone;

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

    // チャンクの床と天井を生成
    for (int32_t x = 0; x < Chunk::k_chunkSizeX - Chunk::k_routeLength; x++) {
        for (int32_t z = 0; z < Chunk::k_chunkSizeZ - Chunk::k_routeLength; z++) {
            table[x][0][z] = k_blockRoomFloor;
            table[x][Chunk::k_chunkSizeY - 1][z] = k_blockRoomRoof;
        }
    }

    // 通路の床と天井と壁を作成
    int32_t rightRouteMinZ = (Chunk::k_chunkSizeZ - Chunk::k_routeLength) / 2 - Chunk::k_routeWidth / 2;
    int32_t rightRouteMaxZ = rightRouteMinZ + Chunk::k_routeWidth;
    for (int32_t x = Chunk::k_chunkSizeX - Chunk::k_routeLength; x < Chunk::k_chunkSizeX; x++) {
        for (int32_t z = rightRouteMinZ; z < rightRouteMaxZ; z++) {
            table[x][0][z] = k_blockRouteFloor;
            table[x][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
        }
        for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
            table[x][y][rightRouteMinZ - 1] = k_blockRouteWall;
            table[x][y][rightRouteMaxZ] = k_blockRouteWall;
        }
    }
    int32_t topRouteMinX = (Chunk::k_chunkSizeX - Chunk::k_routeLength) / 2 - Chunk::k_routeWidth / 2;
    int32_t topRouteMaxX = topRouteMinX + Chunk::k_routeWidth;
    for (int32_t z = Chunk::k_chunkSizeZ - Chunk::k_routeLength; z < Chunk::k_chunkSizeZ; z++) {
        for (int32_t x = topRouteMinX; x < topRouteMaxX; x++) {
            table[x][0][z] = k_blockRouteFloor;
            table[x][Chunk::k_chunkSizeY - 1][z] = k_blockRouteRoof;
        }
        for (int32_t y = 1; y < Chunk::k_chunkSizeY - 1; y++) {
            table[topRouteMinX - 1][y][z] = k_blockRouteWall;
            table[topRouteMaxX][y][z] = k_blockRouteWall;
        }
    }

    // チャンクの壁を生成
    for (int32_t y = 1; y < Chunk::k_chunkSizeY; y++) {
        for (int32_t x = 0; x < Chunk::k_chunkSizeX - Chunk::k_routeLength; x++) {
            if (x >= topRouteMinX && x < topRouteMaxX) {
                continue;
            }
            table[x][y][0] = k_blockRoomWall;
            table[x][y][Chunk::k_chunkSizeZ - Chunk::k_routeLength - 1] = k_blockRoomWall;
        }
        for (int32_t z = 0; z < Chunk::k_chunkSizeZ - Chunk::k_routeLength; z++) {
            if (z >= rightRouteMinZ && z < rightRouteMaxZ) {
                continue;
            }
            table[0][y][z] = k_blockRoomWall;
            table[Chunk::k_chunkSizeX - Chunk::k_routeLength - 1][y][z] = k_blockRoomWall;
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
// private
}