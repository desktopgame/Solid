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
    , m_chunks()
    , m_loadedChunks()
    , m_player()
{
}

void Field::generate()
{
    if (m_chunks.empty()) {
        auto self = shared_from_this();
        {
            auto chunk = std::make_shared<Chunk>(self, IntVector2({ 0, 0 }), m_normalTexture, m_borderTexture);
            chunk->generate();
            m_chunks.emplace_back(chunk);
        }
    }
}

void Field::update()
{
    reloadChunks();

    m_player->update(getCurrentChunk());
    for (auto& chunk : m_loadedChunks) {
        chunk->update();
    }
}
void Field::onGui()
{
    m_player->onGui();

    for (auto& chunk : m_loadedChunks) {
        chunk->onGui();
    }
}
void Field::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    m_player->draw3D(renderer);

    for (auto& chunk : m_loadedChunks) {
        chunk->draw3D(renderer);
    }
}
void Field::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    for (auto& chunk : m_loadedChunks) {
        chunk->draw2D(renderer);
    }
}

void Field::reloadChunks()
{
    m_loadedChunks.clear();

    auto chunk = getCurrentChunk();
    if (chunk) {
        m_loadedChunks.emplace_back(chunk);

        m_loadedChunks.emplace_back(loadChunk(chunk->getGridPosition() + IntVector2({ 1, 0 })));
        m_loadedChunks.emplace_back(loadChunk(chunk->getGridPosition() + IntVector2({ -1, 0 })));
        m_loadedChunks.emplace_back(loadChunk(chunk->getGridPosition() + IntVector2({ 0, 1 })));
        m_loadedChunks.emplace_back(loadChunk(chunk->getGridPosition() + IntVector2({ 0, -1 })));
    }
}

std::shared_ptr<Chunk> Field::getCurrentChunk() const
{
    if (m_player) {
        std::optional<std::shared_ptr<Chunk>> c;
        if (tryFindChunk(c, m_player->getPosition())) {
            return *c;
        }
    }
    return m_chunks.at(0);
}

void Field::setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player) { m_player = player; }
std::shared_ptr<Entities::PlayerEntity> Field::getPlayer() const { return m_player; }
}