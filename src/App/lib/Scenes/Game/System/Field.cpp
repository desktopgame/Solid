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
        {
            auto chunk = std::make_shared<Chunk>(self, IntVector2({ -1, 0 }), m_normalTexture, m_borderTexture);
            chunk->generate();
            m_chunks.emplace_back(chunk);
        }
        {
            auto chunk = std::make_shared<Chunk>(self, IntVector2({ 1, 0 }), m_normalTexture, m_borderTexture);
            chunk->generate();
            m_chunks.emplace_back(chunk);
        }
        {
            auto chunk = std::make_shared<Chunk>(self, IntVector2({ 0, -1 }), m_normalTexture, m_borderTexture);
            chunk->generate();
            m_chunks.emplace_back(chunk);
        }
        {
            auto chunk = std::make_shared<Chunk>(self, IntVector2({ 0, 1 }), m_normalTexture, m_borderTexture);
            chunk->generate();
            m_chunks.emplace_back(chunk);
        }
    }
}

void Field::update()
{
    auto chunk = getCurrentChunk();
    m_player->update(chunk);
    chunk->update();
}
void Field::onGui()
{
    m_player->onGui();
    ImGui::Begin("Chunk");
    ImGui::LabelText("PlayerPos", "%f %f %f", m_player->getPosition().x(), m_player->getPosition().y(), m_player->getPosition().z());
    ImGui::LabelText("Chunk.MinX", "%f", getCurrentChunk()->getPhysicalMinX());
    ImGui::LabelText("Chunk.MixX", "%f", getCurrentChunk()->getPhysicalMaxX());
    ImGui::LabelText("Chunk.MinZ", "%f", getCurrentChunk()->getPhysicalMinZ());
    ImGui::LabelText("Chunk.MixZ", "%f", getCurrentChunk()->getPhysicalMaxZ());
    ImGui::End();
}
void Field::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    m_player->draw3D(renderer);

    for (auto c : m_chunks) {
        c->draw3D(renderer);
    }
}
void Field::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    getCurrentChunk()->draw2D(renderer);
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