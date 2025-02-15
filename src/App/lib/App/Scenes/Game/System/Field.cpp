#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
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

    auto chunk = getCurrentChunk();
    m_player->update(chunk);

    for (auto& chunk : m_loadedChunks) {
        chunk->update();
    }
}
void Field::onGui()
{
    ImGui::Begin("Field");
    {
        auto playerPos = m_player->getPosition();
        ImGui::LabelText("PlayerPos", "%f %f %f", playerPos.x(), playerPos.y(), playerPos.z());
    }
    {
        auto chunk = getCurrentChunk();
        auto chunkGridPos = chunk->getGridPosition();
        ImGui::LabelText("ChunkGridPos", "%d - %d", chunkGridPos.x(), chunkGridPos.y());
        ImGui::LabelText("ChunkMinX", "%f", chunk->getPhysicalMinX());
        ImGui::LabelText("ChunkMaxX", "%f", chunk->getPhysicalMaxX());
        ImGui::LabelText("ChunkMinZ", "%f", chunk->getPhysicalMinZ());
        ImGui::LabelText("ChunkMaxZ", "%f", chunk->getPhysicalMaxZ());
    }
    m_player->onGui();

    for (auto& chunk : m_loadedChunks) {
        chunk->onGui();
    }
    ImGui::End();
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

bool Field::tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const Vector3& pos) const
{
    bool found = false;

    for (auto chunk : m_chunks) {
        float chunkMinX = chunk->getPhysicalMinX();
        float chunkMaxX = chunk->getPhysicalMaxX();
        float chunkMinZ = chunk->getPhysicalMinZ();
        float chunkMaxZ = chunk->getPhysicalMaxZ();

        if (pos.x() >= chunkMinX && pos.x() < chunkMaxX) {
            if (pos.z() >= chunkMinZ && pos.z() < chunkMaxZ) {
                outChunk = chunk;
                found = true;
                break;
            }
        }
    }
    return found;
}

bool Field::tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const IntVector2& gridPosition) const
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

std::shared_ptr<Chunk> Field::loadChunk(const Vector3& pos)
{
    std::optional<std::shared_ptr<Chunk>> c;
    if (tryFindChunk(c, pos)) {
        return *c;
    }
    int32_t tileX = pos.x() / Chunk::k_tileSize;
    int32_t tileZ = pos.z() / Chunk::k_tileSize;

    int32_t gridPosX = tileX / Chunk::k_chunkSizeX;
    if (tileX < 0) {
        gridPosX--;
    }
    int32_t gridPosZ = tileZ / Chunk::k_chunkSizeZ;
    if (tileZ < 0) {
        gridPosZ--;
    }

    auto chunk = std::make_shared<Chunk>(shared_from_this(), IntVector2({ gridPosX, gridPosZ }), m_normalTexture, m_borderTexture);
    chunk->generate();
    m_chunks.emplace_back(chunk);
    return chunk;
}

std::shared_ptr<Chunk> Field::loadChunk(const IntVector2& gridPosition)
{
    std::optional<std::shared_ptr<Chunk>> c;
    if (tryFindChunk(c, gridPosition)) {
        return *c;
    }

    auto chunk = std::make_shared<Chunk>(shared_from_this(), gridPosition, m_normalTexture, m_borderTexture);
    chunk->generate();
    m_chunks.emplace_back(chunk);
    return chunk;
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

std::vector<std::shared_ptr<Chunk>> Field::getLoadedChunks() const
{
    return m_loadedChunks;
}

std::shared_ptr<Chunk> Field::getChunkAt(int32_t index) const { return m_chunks.at(index); }

int32_t Field::getChunkCount() const { return static_cast<int32_t>(m_chunks.size()); }

void Field::setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player) { m_player = player; }
std::shared_ptr<Entities::PlayerEntity> Field::getPlayer() const { return m_player; }
}