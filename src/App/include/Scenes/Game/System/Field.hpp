#pragma once
#include <Scenes/Game/System/Chunk.hpp>
#include <Scenes/Game/System/ChunkGenerator.hpp>
#include <library.hpp>
#include <memory>

namespace App::Scenes::Game::System {
class Field {
public:
    explicit Field(
        const std::shared_ptr<Texture>& normalTexture,
        const std::shared_ptr<Texture>& borderTexture);

    void generate();

    void update();
    void onGui();
    void draw3D(const std::shared_ptr<Renderer>& renderer);
    void draw2D(const std::shared_ptr<Renderer>& renderer);

    inline bool hasBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        return m_chunk->hasBlockAt(x, y, z);
    }

    inline int32_t getBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        return m_chunk->getBlockAt(x, y, z);
    }

    void setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player);
    std::shared_ptr<Entities::PlayerEntity> getPlayer() const;

    void spwan(const std::shared_ptr<Entity>& entity);
    std::shared_ptr<Entity> getEntityAt(int32_t index) const;
    int32_t getEntityCount() const;

    ChunkGenerator::Room getRoomAt(int32_t index) const;
    int32_t getRoomCount() const;

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
    std::shared_ptr<Chunk> m_chunk;
    std::shared_ptr<Entities::PlayerEntity> m_player;
};
}