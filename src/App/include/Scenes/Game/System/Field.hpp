#pragma once
#include <Scenes/Game/System/Chunk.hpp>
#include <Scenes/Game/System/ChunkGenerator.hpp>
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Game::System {
class Field : public std::enable_shared_from_this<Field> {
public:
    explicit Field(
        const std::shared_ptr<Texture>& normalTexture,
        const std::shared_ptr<Texture>& borderTexture);

    void generate();

    void update();
    void onGui();
    void draw3D(const std::shared_ptr<Renderer>& renderer);
    void draw2D(const std::shared_ptr<Renderer>& renderer);

    inline bool tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const Vector3& pos) const
    {
        int32_t ix = static_cast<int32_t>(pos.x());
        int32_t iz = static_cast<int32_t>(pos.z());
        bool found = false;

        for (auto chunk : m_chunks) {
            IntVector2 chunkPos = chunk->getGridPosition();
            float chunkCenterX = (chunkPos.x() * Chunk::k_fieldSizeX) + (Chunk::k_fieldSizeX / 2) * Chunk::k_tileSize;
            float chunkCenterZ = (chunkPos.y() * Chunk::k_fieldSizeZ) + (Chunk::k_fieldSizeZ / 2) * Chunk::k_tileSize;
            float chunkMinX = chunkCenterX - ((Chunk::k_fieldSizeX * Chunk::k_tileSize) / 2.0f);
            float chunkMaxX = chunkCenterX + ((Chunk::k_fieldSizeX * Chunk::k_tileSize) / 2.0f);
            float chunkMinZ = chunkCenterZ - ((Chunk::k_fieldSizeZ * Chunk::k_tileSize) / 2.0f);
            float chunkMaxZ = chunkCenterZ + ((Chunk::k_fieldSizeZ * Chunk::k_tileSize) / 2.0f);

            if (pos.x() >= chunkMinX && pos.x() <= chunkMaxX) {
                if (pos.z() >= chunkMinZ && pos.z() <= chunkMaxZ) {
                    outChunk = chunk;
                    found = true;
                    break;
                }
            }
        }
        return found;
    }

    std::shared_ptr<Chunk> getCurrentChunk() const;

    inline bool hasBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        std::optional<std::shared_ptr<Chunk>> c;
        float fx = static_cast<float>(x) * Chunk::k_tileSize;
        float fy = static_cast<float>(y) * Chunk::k_tileSize;
        float fz = static_cast<float>(z) * Chunk::k_tileSize;
        if (tryFindChunk(c, Vector3({ fx, fy, fz }))) {
            return (*c)->hasBlockAt(x, y, z);
        }
        return false;
    }

    inline int32_t getBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        std::optional<std::shared_ptr<Chunk>> c;
        float fx = static_cast<float>(x) * Chunk::k_tileSize;
        float fy = static_cast<float>(y) * Chunk::k_tileSize;
        float fz = static_cast<float>(z) * Chunk::k_tileSize;
        if (tryFindChunk(c, Vector3({ fx, fy, fz }))) {
            return (*c)->getBlockAt(x, y, z);
        }
        return 0;
    }

    void setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player);
    std::shared_ptr<Entities::PlayerEntity> getPlayer() const;

    static inline int32_t toIndex(int32_t x, int32_t y, int32_t z)
    {
        return Chunk::toIndex(x, y, z);
    }
    static inline IntVector3 toCoord(int32_t index)
    {
        return Chunk::toCoord(index);
    }

private:
    std::shared_ptr<Texture> m_normalTexture;
    std::shared_ptr<Texture> m_borderTexture;
    std::vector<std::shared_ptr<Chunk>> m_chunks;
    std::shared_ptr<Entities::PlayerEntity> m_player;
};
}