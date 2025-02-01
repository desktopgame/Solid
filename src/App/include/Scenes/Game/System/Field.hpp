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
        bool found = false;

        for (auto chunk : m_chunks) {
            float chunkMinX = chunk->getPhysicalMinX();
            float chunkMaxX = chunk->getPhysicalMaxX();
            float chunkMinZ = chunk->getPhysicalMinZ();
            float chunkMaxZ = chunk->getPhysicalMaxZ();

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

    inline bool tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const IntVector2& gridPosition) const
    {
        bool found = false;

        for (auto& chunk : m_chunks) {
            if (chunk->getGridPosition() == gridPosition) {
                outChunk = chunk;
                found = true;
                break;
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
            x -= (*c)->getGridPosition().x() * Chunk::k_fieldSizeX;
            z -= (*c)->getGridPosition().y() * Chunk::k_fieldSizeZ;
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
            x -= (*c)->getGridPosition().x() * Chunk::k_fieldSizeX;
            z -= (*c)->getGridPosition().y() * Chunk::k_fieldSizeZ;
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
    std::shared_ptr<Entities::PlayerEntity> m_player;
};
}