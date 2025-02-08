#include <Scenes/Game/System/Minimap.hpp>

namespace App::Scenes::Game::System {
// public
Minimap::Minimap(const std::shared_ptr<Field>& field)
    : m_field(field)
{
}

void Minimap::update()
{
}

void Minimap::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    auto player = m_field->getPlayer();
    if (!m_field || !player) {
        return;
    }

    renderer->drawRect(k_backgroundCenter, k_backgroundSize, 0.0f, Vector4({ 1, 1, 1, 0.5f }));

    const Vector2 k_chunkSize({
        (k_backgroundSize.x() - (k_routeLength * 4)) / 3,
        (k_backgroundSize.y() - (k_routeLength * 4)) / 3,
    });

    std::vector<std::shared_ptr<Chunk>> chunks;
    auto playerPos = player->getPosition();
    auto currentChunk = m_field->getCurrentChunk();
    auto currentChunkGridPos = currentChunk->getGridPosition();
    chunks.emplace_back(currentChunk);
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ -1, 0 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ 1, 0 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ 0, -1 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ 0, 1 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ -1, -1 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ 1, 1 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ -1, 1 })));
    chunks.emplace_back(m_field->loadChunk(currentChunkGridPos + IntVector2({ 1, -1 })));

    auto mapScrollAmount = (Vector3({ playerPos.x(), 0, playerPos.z() }) - Vector3({ currentChunk->getPhysicalCenterX(), 0, currentChunk->getPhysicalCenterZ() })) / Chunk::k_tileSize;
    mapScrollAmount.x() *= (k_chunkSize.x()) / static_cast<float>(Chunk::k_chunkSizeX - Chunk::k_routeLength);
    mapScrollAmount.z() *= (k_chunkSize.y()) / static_cast<float>(Chunk::k_chunkSizeZ - Chunk::k_routeLength);

    for (const auto chunk : chunks) {
        IntVector2 gridPos = chunk->getGridPosition();
        IntVector2 localGridPos = currentChunkGridPos - gridPos;

        int32_t col = localGridPos.x() + 1;
        int32_t row = localGridPos.y() + 1;

        float offsetX = ((col + 1) * k_routeLength) + (col * k_chunkSize.x()) + (k_chunkSize.x() / 2.0f) - (k_backgroundSize.x() / 2.0f);
        float offsetY = ((row + 1) * k_routeLength) + (row * k_chunkSize.y()) + (k_chunkSize.y() / 2.0f) - (k_backgroundSize.y() / 2.0f);

        offsetX += mapScrollAmount.x();
        offsetY += mapScrollAmount.z();

        renderer->drawRect(
            k_backgroundCenter + Vector2({ offsetX, offsetY }), k_chunkSize, 0.0f, Vector4({ 0, 0, 0, 1 }));
    }

    renderer->drawRect(
        k_backgroundCenter, Vector2({ 5, 5 }), 0.0f, Vector4({ 0, 1, 0, 1 }));
}
}