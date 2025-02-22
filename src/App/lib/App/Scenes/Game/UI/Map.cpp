#include <App/Scenes/Game/System/PieceInfo.hpp>
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
    , m_pieceInfo()
    , m_pieceInstanceCollection()
    , m_focusCells()
    , m_focusChunkX()
    , m_focusChunkY()
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
    m_chunkCountX = (m_maxChunkX - m_minChunkX) + 1;
    m_chunkCountY = (m_maxChunkY - m_minChunkY) + 1;
    setPreferredSize(Vector2({ //
        (m_chunkCountX * k_chunkWidth) + ((m_chunkCountX + 1) * k_routeSize),
        (m_chunkCountY * k_chunkHeight) + ((m_chunkCountX + 1) * k_routeSize) }));
}

void Map::update()
{
    auto inputSystem = InputSystem::getInstance();
    auto mousePos = inputSystem->getMouse()->getPosition();

    auto screenPos = (((Vector2)mousePos / (Vector2)Screen::getSize()) - Vector2({ 0.5f, 0.5f })) * (Vector2)Screen::getSize();
    screenPos.y() *= -1;

    auto size = getSize();
    auto center = getGlobalPosition();
    float left = center.x() - (size.x() / 2.0f);
    float top = center.y() + (size.y() / 2.0f);

    m_focusCells.clear();

    // フォーカスされているチャンクを検索する
    m_focusChunkX = std::nullopt;
    m_focusChunkY = std::nullopt;
    for (int32_t x = 0; x < m_chunkCountX; x++) {
        for (int32_t y = 0; y < m_chunkCountY; y++) {
            float routeOffsetX = static_cast<float>(x + 1) * k_routeSize;
            float routeOffsetY = static_cast<float>(y + 1) * k_routeSize;
            float chunkOffsetX = (x * k_chunkWidth) + (k_chunkWidth / 2.0f);
            float chunkOffsetY = (y * k_chunkHeight) + (k_chunkHeight / 2.0f);

            auto rectPos = Vector2({ //
                left + routeOffsetX + chunkOffsetX,
                top - (routeOffsetY + chunkOffsetY) });
            auto rectSize = Vector2({ //
                k_chunkWidth,
                k_chunkHeight });

            bool hasFocus = true;
            if (screenPos.x() < rectPos.x() - (rectSize.x() / 2.0f) || screenPos.x() > rectPos.x() + (rectSize.x() / 2.0f)) {
                hasFocus = false;
            }
            if (screenPos.y() < rectPos.y() - (rectSize.y() / 2.0f) || screenPos.y() > rectPos.y() + (rectSize.y() / 2.0f)) {
                hasFocus = false;
            }
            if (hasFocus) {
                m_focusChunkX = x;
                m_focusChunkY = y;
                break;
            }
        }
    }

    // フォーカス状態を検出
    for (int32_t x = 0; x < m_chunkCountX; x++) {
        for (int32_t y = 0; y < m_chunkCountY; y++) {
            int32_t chunkGridPosX = m_minChunkX + x;
            int32_t chunkGridPosY = m_minChunkY + (m_chunkCountY - (y + 1));

            std::optional<std::shared_ptr<System::Chunk>> optChunk;
            m_field->tryFindChunk(optChunk, IntVector2({ chunkGridPosX, chunkGridPosY }));
            if (!optChunk) {
                continue;
            }

            if (!m_pieceInfo || !m_focusChunkX || !m_focusChunkY) {
                continue;
            }

            // フォーカスされたチャンクを記憶
            bool coordMatchOk = false;
            for (const auto& cell : m_pieceInfo->getCells()) {
                int32_t cellX = cell.position.x() + (*m_focusChunkX);
                int32_t cellY = cell.position.y() + (*m_focusChunkY);
                if (x == cellX && y == cellY) {
                    coordMatchOk = true;
                    break;
                }
            }
            bool existOk = true;
            if (coordMatchOk) {
                for (const auto& cell : m_pieceInfo->getCells()) {
                    int32_t cellChunkGridPosX = m_minChunkX + (cell.position.x() + *m_focusChunkX);
                    int32_t cellChunkGridPosY = m_minChunkY + (m_chunkCountY - (*m_focusChunkY + 1)) - cell.position.y();

                    std::optional<std::shared_ptr<System::Chunk>> atChunk;
                    m_field->tryFindChunk(atChunk, IntVector2({ cellChunkGridPosX, cellChunkGridPosY }));
                    if (!atChunk) {
                        existOk = false;
                        break;
                    }
                    if ((*atChunk)->countEntity(System::Entity::Category::Enemy) > 0) {
                        existOk = false;
                        break;
                    }
                }
            }
            if (coordMatchOk && existOk) {
                m_focusCells.emplace_back(IntVector2({ x, y }));
            }
        }
    }
}

void Map::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    auto size = getSize();
    auto center = getGlobalPosition();
    float left = center.x() - (size.x() / 2.0f);
    float top = center.y() + (size.y() / 2.0f);
    // ロード済みのチャンクを描画
    for (int32_t x = 0; x < m_chunkCountX; x++) {
        for (int32_t y = 0; y < m_chunkCountY; y++) {
            float routeOffsetX = static_cast<float>(x + 1) * k_routeSize;
            float routeOffsetY = static_cast<float>(y + 1) * k_routeSize;
            float chunkOffsetX = (x * k_chunkWidth) + (k_chunkWidth / 2.0f);
            float chunkOffsetY = (y * k_chunkHeight) + (k_chunkHeight / 2.0f);

            int32_t chunkGridPosX = m_minChunkX + x;
            int32_t chunkGridPosY = m_minChunkY + (m_chunkCountY - (y + 1));

            std::optional<std::shared_ptr<System::Chunk>> optChunk;
            m_field->tryFindChunk(optChunk, IntVector2({ chunkGridPosX, chunkGridPosY }));
            if (!optChunk) {
                continue;
            }

            auto chunk = *optChunk;
            Color chunkColor = Color({ 1, 1, 1, 1 });
            if (!chunk) {
                chunkColor = Color({ 0.1f, 0.1f, 0.1f, 1 });
            } else {
                int32_t enemyCount = chunk->countEntity(System::Entity::Category::Enemy);
                if (enemyCount > 0) {
                    chunkColor = k_enemyTeamChunkColor;
                } else {
                    chunkColor = k_playerTeamChunkColor;
                }
            }

            // フォーカスされたチャンクなら色変更
            if (m_pieceInfo) {
                auto iter = std::find(m_focusCells.begin(), m_focusCells.end(), IntVector2({ x, y }));
                if (iter != m_focusCells.end()) {
                    chunkColor = k_focusChunkColor;
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

    // プレイヤーを描画
    auto player = m_field->getPlayer();
    auto playerPos = player->getPosition();
    auto leftChunkX = System::Chunk::getPhysicalMinX(m_minChunkX);
    auto topChunkZ = System::Chunk::getPhysicalMaxZ(m_maxChunkY);

    auto offsetX = (playerPos.x() - leftChunkX) / System::Chunk::k_tileSize;
    auto offsetY = (playerPos.z() - topChunkZ) / System::Chunk::k_tileSize;
    offsetX *= (k_chunkWidth / static_cast<float>(System::Chunk::k_chunkSizeX - System::Chunk::k_routeLength));
    offsetY *= (k_chunkHeight / static_cast<float>(System::Chunk::k_chunkSizeZ - System::Chunk::k_routeLength));

    renderer->drawRect(
        Vector2({
            left + offsetX,
            top + offsetY,
        }),
        Vector2({ //
            5.0f,
            5.0f }),
        0.0f, Vector4({ 0, 1, 0, 1 }));
}

void Map::setPieceInfo(const std::shared_ptr<System::PieceInfo>& pieceInfo) { m_pieceInfo = pieceInfo; }
std::shared_ptr<System::PieceInfo> Map::getPieceInfo() const { return m_pieceInfo; }

void Map::setPieceInstanceCollection(const std::shared_ptr<System::PieceInstanceCollection>& pieceInstanceCollection) { m_pieceInstanceCollection = pieceInstanceCollection; }
std::shared_ptr<System::PieceInstanceCollection> Map::getPieceInstanceCollection() const { return m_pieceInstanceCollection; }
}