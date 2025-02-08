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

    std::vector<IntVector2> localGridPositions;
    auto playerPos = player->getPosition();
    auto currentChunk = m_field->getCurrentChunk();
    auto currentChunkGridPos = currentChunk->getGridPosition();
    localGridPositions.emplace_back(IntVector2({ 0, 0 }));

    IntVector2 startGridPos = localGridPositions.front() - IntVector2({ 2, 2 });
    for (int32_t i = 0; i <= 4; i++) {
        for (int32_t j = 0; j <= 4; j++) {
            localGridPositions.emplace_back(startGridPos + IntVector2({ i, j }));
        }
    }

    auto mapScrollAmount = -(Vector3({ playerPos.x(), 0, playerPos.z() }) - Vector3({ currentChunk->getPhysicalCenterX(), 0, currentChunk->getPhysicalCenterZ() })) / Chunk::k_tileSize;
    mapScrollAmount.x() *= (k_chunkSize.x()) / static_cast<float>(Chunk::k_chunkSizeX - Chunk::k_routeLength);
    mapScrollAmount.z() *= (k_chunkSize.y()) / static_cast<float>(Chunk::k_chunkSizeZ - Chunk::k_routeLength);

    for (const auto localGridPos : localGridPositions) {

        int32_t col = localGridPos.x() + 1;
        int32_t row = localGridPos.y() + 1;

        float offsetX = ((col + 1) * k_routeLength) + (col * k_chunkSize.x()) + (k_chunkSize.x() / 2.0f) - (k_backgroundSize.x() / 2.0f);
        float offsetY = ((row + 1) * k_routeLength) + (row * k_chunkSize.y()) + (k_chunkSize.y() / 2.0f) - (k_backgroundSize.y() / 2.0f);

        offsetX += mapScrollAmount.x();
        offsetY += mapScrollAmount.z();

        if (Mathf::abs(offsetX) - k_chunkSize.x() / 2.0f >= k_backgroundSize.x() / 2.0f || Mathf::abs(offsetY) - k_chunkSize.y() / 2.0f >= k_backgroundSize.y() / 2.0f) {
            continue;
        }

        auto chunk = m_field->loadChunk(currentChunk->getGridPosition() + localGridPos);
        renderer->drawRect(
            k_backgroundCenter + Vector2({ offsetX, offsetY }), k_chunkSize, 0.0f, Vector4({ 0, 0, 0, 1 }));

        for (int32_t i = 0; i < chunk->getEntityCount(); i++) {
            auto entity = chunk->getEntityAt(i);
            auto entityPos = entity->getPosition();

            auto entityOffset = (entityPos - Vector3({ chunk->getPhysicalCenterX(), 0, chunk->getPhysicalCenterZ() })) / Chunk::k_tileSize;
            entityOffset.x() *= (k_chunkSize.x()) / static_cast<float>(Chunk::k_chunkSizeX - Chunk::k_routeLength);
            entityOffset.z() *= (k_chunkSize.y()) / static_cast<float>(Chunk::k_chunkSizeZ - Chunk::k_routeLength);

            entityOffset.x() += offsetX;
            entityOffset.z() += offsetY;

            if (Mathf::abs(entityOffset.x()) - 5.0f / 2.0f >= k_backgroundSize.x() / 2.0f || Mathf::abs(entityOffset.z()) - 5.0f / 2.0f >= k_backgroundSize.y() / 2.0f) {
                continue;
            }
            renderer->drawRect(
                k_backgroundCenter + Vector2({ entityOffset.x(), entityOffset.z() }), Vector2({ 5, 5 }), 0.0f, Vector4({ 1, 0, 0, 1 }));
        }
    }

    renderer->drawRect(
        k_backgroundCenter, Vector2({ 5, 5 }), 0.0f, Vector4({ 0, 1, 0, 1 }));
}
}