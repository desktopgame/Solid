#include <App/Scenes/Game/UI/Map.hpp>

namespace App::Scenes::Game::UI {

Map::Map(const std::shared_ptr<System::Field>& field)
    : Component()
    , m_field(field)
    , m_minChunkX()
    , m_maxChunkX()
    , m_minChunkY()
    , m_maxChunkY()
    , m_chunkCountX()
    , m_chunkCountY()
{
}

void Map::setup()
{
    m_minChunkX = 999;
    m_maxChunkX = -999;
    m_minChunkY = 999;
    m_maxChunkY = -999;

    for (int32_t i = 0; i < m_field->getChunkCount(); i++) {
        auto chunk = m_field->getChunkAt(i);
        auto gridPos = chunk->getGridPosition();

        if (gridPos.x() < m_minChunkX) {
            m_minChunkX = gridPos.x();
        }
        if (gridPos.x() > m_maxChunkX) {
            m_maxChunkX = gridPos.x();
        }
        if (gridPos.y() < m_minChunkY) {
            m_minChunkY = gridPos.y();
        }
        if (gridPos.y() > m_maxChunkY) {
            m_maxChunkY = gridPos.y();
        }
    }
    m_chunkCountX = m_maxChunkX - m_minChunkX;
    m_chunkCountY = m_maxChunkY - m_minChunkY;
    setPreferredSize(Vector2({ //
        (m_chunkCountX * k_chunkWidth) + ((m_chunkCountX + 1) * k_routeSize),
        (m_chunkCountY * k_chunkHeight) + ((m_chunkCountX + 1) * k_routeSize) }));
}

void Map::update() { }
void Map::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    auto size = getSize();
    auto center = getGlobalPosition();
    auto left = center.x() - (size.x() / 2.0f);
    auto top = center.y() + (size.y() / 2.0f);
    // ロード済みのチャンクを描画
    for (int32_t x = 0; x < m_chunkCountX; x++) {
        for (int32_t y = 0; y < m_chunkCountY; y++) {
            float routeOffsetX = static_cast<float>(x + 1) * k_routeSize;
            float routeOffsetY = static_cast<float>(y + 1) * k_routeSize;
            float chunkOffsetX = (x * k_chunkWidth) + (k_chunkWidth / 2.0f);
            float chunkOffsetY = (y * k_chunkHeight) + (k_chunkHeight / 2.0f);

            int32_t chunkGridPosX = m_minChunkX + x;
            int32_t chunkGridPosY = m_minChunkY + (m_chunkCountY - y);
            auto chunk = m_field->loadChunk(IntVector2({ chunkGridPosX, chunkGridPosY }));

            Color chunkColor = Color({ 1, 1, 1, 1 });
            if (!chunk) {
                chunkColor = Color({ 0.1f, 0.1f, 0.1f, 1 });
            } else {
                int32_t enemyCount = 0;
                for (int32_t i = 0; i < chunk->getEntityCount(); i++) {
                    auto entity = chunk->getEntityAt(i);
                    if (entity->getCategory() == System::Entity::Category::Enemy) {
                        enemyCount++;
                    }
                }

                if (enemyCount > 0) {
                    chunkColor = Vector4({ 0.5f, 0, 0, 1 });
                } else {
                    chunkColor = Vector4({ 0, 0.5f, 0, 1 });
                }
            }

            auto rectPos = Vector2({ //
                left + routeOffsetX + chunkOffsetX,
                top - (routeOffsetY + chunkOffsetY) });
            auto rectSize = Vector2({ //
                k_chunkWidth,
                k_chunkHeight });
            renderer->drawRect(rectPos, rectSize, 0.0f, chunkColor);

            // チャンク内のエンティティも描画
            for (int32_t i = 0; i < chunk->getEntityCount(); i++) {
                auto entity = chunk->getEntityAt(i);
                auto entityPos = entity->getPosition();

                if (entity->getCategory() == System::Entity::Category::PlayerTeam || entity->getCategory() == System::Entity::Category::EnemyTeam) {
                    continue;
                }

                float entityOffsetX = (entityPos.x() - chunk->getPhysicalCenterX()) / System::Chunk::k_tileSize;
                float entityOffsetZ = (entityPos.z() - chunk->getPhysicalCenterZ()) / System::Chunk::k_tileSize;
                entityOffsetX *= (k_chunkWidth / static_cast<float>(System::Chunk::k_chunkSizeX - System::Chunk::k_routeLength));
                entityOffsetZ *= (k_chunkHeight / static_cast<float>(System::Chunk::k_chunkSizeX - System::Chunk::k_routeLength));

                renderer->drawRect(
                    Vector2({ //
                        (left + routeOffsetX + chunkOffsetX) + entityOffsetX,
                        (top - (routeOffsetY + chunkOffsetY)) + entityOffsetZ }),
                    Vector2({ //
                        5.0f,
                        5.0f }),
                    0.0f, Vector4({ 1, 0, 0, 1 }));
            }
        }
    }
}
}