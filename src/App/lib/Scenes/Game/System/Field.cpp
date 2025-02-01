#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <algorithm>
#include <imgui.h>

namespace App::Scenes::Game::System {
// public
Field::Field(
    const std::shared_ptr<Texture>& normalTexture,
    const std::shared_ptr<Texture>& borderTexture)
    : m_normalTexture(normalTexture)
    , m_borderTexture(borderTexture)
    , m_chunk()
    , m_player()
{
}

void Field::generate()
{
    if (!m_chunk) {
        m_chunk = std::make_shared<Chunk>(*this, IntVector2({ 0, 0 }), m_normalTexture, m_borderTexture);
        m_chunk->generate();
    }
}

void Field::update()
{
    m_player->update(*this);
    m_chunk->update();
}
void Field::onGui()
{
    m_player->onGui();
    m_chunk->onGui();
}
void Field::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    m_player->draw3D(renderer);
    m_chunk->draw3D(renderer);
}
void Field::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    m_chunk->draw2D(renderer);
}

void Field::setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player) { m_player = player; }
std::shared_ptr<Entities::PlayerEntity> Field::getPlayer() const { return m_player; }

void Field::spwan(const std::shared_ptr<Entity>& entity) { m_chunk->spwan(entity); }
std::shared_ptr<Entity> Field::getEntityAt(int32_t index) const { return m_chunk->getEntityAt(index); }
int32_t Field::getEntityCount() const { return m_chunk->getEntityCount(); }

ChunkGenerator::Room Field::getRoomAt(int32_t index) const { return m_chunk->getRoomAt(index); }
int32_t Field::getRoomCount() const { return m_chunk->getRoomCount(); }
}