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

    bool tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const Vector3& pos) const;

    bool tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const IntVector2& gridPosition) const;

    std::shared_ptr<Chunk> loadChunk(const Vector3& pos);

    std::shared_ptr<Chunk> loadChunk(const IntVector2& gridPosition);

    void reloadChunks();

    std::shared_ptr<Chunk> getCurrentChunk() const;

    std::vector<std::shared_ptr<Chunk>> getLoadedChunks() const;

    static inline IntVector3 toLocalBlockPosition(int32_t x, int32_t y, int32_t z)
    {
        int32_t gridPosX = x / Chunk::k_chunkSizeX;
        if (x < 0) {
            gridPosX--;
        }
        int32_t gridPosZ = z / Chunk::k_chunkSizeZ;
        if (z < 0) {
            gridPosZ--;
        }
        int32_t localX = x;
        localX -= gridPosX * Chunk::k_chunkSizeX;
        int32_t localZ = z;
        localZ -= gridPosZ * Chunk::k_chunkSizeZ;
        return IntVector3({ localX, y, localZ });
    }

    static inline IntVector3 toGlobalBlockPosition(const IntVector2& gridPosition, int32_t x, int32_t y, int32_t z)
    {
        int32_t globalX = x;
        if (gridPosition.x() > 0) {
            globalX += gridPosition.x() * Chunk::k_chunkSizeX;
        } else if (gridPosition.x() < 0) {
            x = Chunk::k_chunkSizeX - x;
            globalX = (gridPosition.x() * Chunk::k_chunkSizeX) + x;
        }
        int32_t globalZ = z;
        if (gridPosition.y() > 0) {
            globalZ += gridPosition.y() * Chunk::k_chunkSizeZ;
        } else if (gridPosition.y() < 0) {
            z = Chunk::k_chunkSizeZ - z;
            globalZ = (gridPosition.y() * Chunk::k_chunkSizeZ) + z;
        }
        return IntVector3({ globalX, y, globalZ });
    }

    inline bool hasBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        std::optional<std::shared_ptr<Chunk>> c;
        float fx = static_cast<float>(x) * Chunk::k_tileSize;
        float fy = static_cast<float>(y) * Chunk::k_tileSize;
        float fz = static_cast<float>(z) * Chunk::k_tileSize;
        if (tryFindChunk(c, Vector3({ fx, fy, fz }))) {
            IntVector2 gridPos = (*c)->getGridPosition();
            x -= gridPos.x() * Chunk::k_chunkSizeX;
            z -= gridPos.y() * Chunk::k_chunkSizeZ;
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
            IntVector2 gridPos = (*c)->getGridPosition();
            x -= gridPos.x() * Chunk::k_chunkSizeX;
            z -= gridPos.y() * Chunk::k_chunkSizeZ;
            return (*c)->getBlockAt(x, y, z);
        }
        return 0;
    }

    void setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player);
    std::shared_ptr<Entities::PlayerEntity> getPlayer() const;

private:
    std::shared_ptr<Texture> m_normalTexture;
    std::shared_ptr<Texture> m_borderTexture;
    std::vector<std::shared_ptr<Chunk>> m_chunks;
    std::vector<std::shared_ptr<Chunk>> m_loadedChunks;
    std::shared_ptr<Entities::PlayerEntity> m_player;
};
}