#include <Scenes/Game/System/Minimap.hpp>

namespace App::Scenes::Game::System {
// public
Minimap::Minimap(const std::shared_ptr<Field>& field)
    : m_field(field)
    , m_position(Vector2({ 150.0f, 150.0f })) // 左上から150pxの位置(中心座標)
    , m_size(Vector2({ 200.0f, 200.0f })) // 200x200のサイズ
{
}

void Minimap::update()
{
    auto field = m_field.lock();
    if (!field) {
        return;
    }

    auto player = field->getPlayer();
    if (!player) {
        return;
    }

    const auto& playerPos = player->getPosition();
    const float mapScale = m_size.x() / (Chunk::k_chunkSizeX * Chunk::k_tileSize);
    const float visibleRange = m_size.x() / (2.0f * mapScale); // ミニマップに表示される範囲(物理座標)

    // プレイヤーの位置からグリッド座標を計算
    const int32_t gridX = static_cast<int32_t>(playerPos.x() / (Chunk::k_chunkSizeX * Chunk::k_tileSize));
    const int32_t gridZ = static_cast<int32_t>(playerPos.z() / (Chunk::k_chunkSizeZ * Chunk::k_tileSize));

    // ミニマップに表示される範囲のチャンクをロード
    const int32_t chunkRange = static_cast<int32_t>(visibleRange / (Chunk::k_chunkSizeX * Chunk::k_tileSize)) + 1;

    for (int32_t x = -chunkRange; x <= chunkRange; ++x) {
        for (int32_t z = -chunkRange; z <= chunkRange; ++z) {
            IntVector2 gridPos({ gridX + x, gridZ + z });
            field->loadChunk(gridPos);
        }
    }
}

void Minimap::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    auto field = m_field.lock();
    if (!field) {
        return;
    }

    // 現在のチャンクを取得
    auto currentChunk = field->getCurrentChunk();
    if (!currentChunk) {
        return;
    }

    auto player = field->getPlayer();
    if (!player) {
        return;
    }

    const auto& playerPos = player->getPosition();
    const float mapScale = m_size.x() / (Chunk::k_chunkSizeX * Chunk::k_tileSize);
    const float visibleRange = m_size.x() / (2.0f * mapScale); // ミニマップに表示される範囲(物理座標)

    // プレイヤーの位置からグリッド座標を計算
    const int32_t gridX = static_cast<int32_t>(playerPos.x() / (Chunk::k_chunkSizeX * Chunk::k_tileSize));
    const int32_t gridZ = static_cast<int32_t>(playerPos.z() / (Chunk::k_chunkSizeZ * Chunk::k_tileSize));
    const int32_t chunkRange = static_cast<int32_t>(visibleRange / (Chunk::k_chunkSizeX * Chunk::k_tileSize)) + 1;

    // ミニマップの背景を描画
    renderer->drawRect(
        m_position, // 中心位置
        m_size,
        0.0f, // 回転なし
        Color({ 0.0f, 0.0f, 0.0f, 0.5f })); // 半透明の黒

    // チャンクの境界を描画
    renderer->drawRect(
        m_position, // 中心位置
        m_size,
        0.0f, // 回転なし
        Color({ 1.0f, 1.0f, 1.0f, 0.3f })); // 半透明の白

    // 表示範囲内の全てのチャンクの部屋とエンティティを描画
    for (int32_t x = -chunkRange; x <= chunkRange; ++x) {
        for (int32_t z = -chunkRange; z <= chunkRange; ++z) {
            IntVector2 gridPos({ gridX + x, gridZ + z });
            std::optional<std::shared_ptr<Chunk>> chunk;
            if (field->tryFindChunk(chunk, gridPos)) {
                // 部屋を描画
                drawRooms(renderer, *chunk, playerPos);

                // エンティティを描画
                for (int32_t i = 0; i < (*chunk)->getEntityCount(); ++i) {
                    auto entity = (*chunk)->getEntityAt(i);
                    if (!entity) {
                        continue;
                    }
                    drawEntity(renderer, entity, *chunk, playerPos, Color({ 1.0f, 0.0f, 0.0f, 1.0f })); // 赤色
                }
            }
        }
    }

    // プレイヤーを中央に描画
    renderer->drawRect(
        m_position, // 中心位置
        Vector2({ 5.0f, 5.0f }), // エンティティサイズ
        0.0f,
        Color({ 0.0f, 1.0f, 0.0f, 1.0f })); // 緑色
}
// private
bool Minimap::isInMinimapRange(const Vector2& mapPos, const Vector2& size) const
{
    // ミニマップの表示範囲をチェック
    const float halfWidth = m_size.x() * 0.5f;
    const float halfHeight = m_size.y() * 0.5f;
    const float objHalfWidth = size.x() * 0.5f;
    const float objHalfHeight = size.y() * 0.5f;

    // オブジェクトが完全にミニマップの範囲内に収まる場合のみtrue
    return (mapPos.x() + objHalfWidth <= m_position.x() + halfWidth && mapPos.x() - objHalfWidth >= m_position.x() - halfWidth && mapPos.y() + objHalfHeight <= m_position.y() + halfHeight && mapPos.y() - objHalfHeight >= m_position.y() - halfHeight);
}

void Minimap::drawEntity(
    const std::shared_ptr<Renderer>& renderer,
    const std::shared_ptr<Entity>& entity,
    const std::shared_ptr<Chunk>& chunk,
    const Vector3& playerPos,
    const Color& color) const
{
    const auto& entityPos = entity->getPosition();
    // エンティティの位置がチャンクの範囲内かチェック
    if (entityPos.x() < chunk->getPhysicalMinX() || entityPos.x() > chunk->getPhysicalMaxX() || entityPos.z() < chunk->getPhysicalMinZ() || entityPos.z() > chunk->getPhysicalMaxZ()) {
        return; // チャンクの範囲外なのでスキップ
    }

    // プレイヤーからの相対位置を計算
    const float relativeX = entityPos.x() - playerPos.x();
    const float relativeZ = entityPos.z() - playerPos.z();

    // ミニマップ上の位置を計算
    const float mapScale = m_size.x() / (Chunk::k_chunkSizeX * Chunk::k_tileSize);
    const Vector2 entitySize({ 5.0f, 5.0f });
    Vector2 mapPos = Vector2({ m_position.x() + (relativeX * mapScale),
        m_position.y() + (relativeZ * mapScale) });

    // ミニマップの範囲内かチェック
    if (!isInMinimapRange(mapPos, entitySize)) {
        return; // 範囲外なのでスキップ
    }

    // エンティティを描画
    renderer->drawRect(
        mapPos,
        entitySize,
        0.0f, // 回転なし
        color);
}

void Minimap::drawRooms(
    const std::shared_ptr<Renderer>& renderer,
    const std::shared_ptr<Chunk>& chunk,
    const Vector3& playerPos) const
{
    const float mapScale = m_size.x() / (Chunk::k_chunkSizeX * Chunk::k_tileSize);

    for (int32_t i = 0; i < chunk->getRoomCount(); ++i) {
        auto room = chunk->getRoomAt(i);

        // 部屋のローカル座標をグローバル座標に変換
        const auto gridPos = chunk->getGridPosition();
        const float roomX = (gridPos.x() * Chunk::k_chunkSizeX + room.center.x()) * Chunk::k_tileSize;
        const float roomZ = (gridPos.y() * Chunk::k_chunkSizeZ + room.center.z()) * Chunk::k_tileSize;

        // プレイヤーからの相対位置を計算
        const float relativeX = roomX - playerPos.x();
        const float relativeZ = roomZ - playerPos.z();

        Vector2 mapPos = Vector2({ m_position.x() + (relativeX * mapScale),
            m_position.y() + (relativeZ * mapScale) });

        // 部屋のサイズをミニマップスケールに変換
        const Vector2 roomSize({ Chunk::k_roomSizeX * Chunk::k_tileSize * mapScale,
            Chunk::k_roomSizeZ * Chunk::k_tileSize * mapScale });

        // ミニマップの範囲に合わせてサイズと位置を調整
        const float halfWidth = m_size.x() * 0.5f;
        const float halfHeight = m_size.y() * 0.5f;
        const float roomHalfWidth = roomSize.x() * 0.5f;
        const float roomHalfHeight = roomSize.y() * 0.5f;

        // 部屋がミニマップの範囲外に完全に出ている場合はスキップ
        if (mapPos.x() + roomHalfWidth < m_position.x() - halfWidth || mapPos.x() - roomHalfWidth > m_position.x() + halfWidth || mapPos.y() + roomHalfHeight < m_position.y() - halfHeight || mapPos.y() - roomHalfHeight > m_position.y() + halfHeight) {
            continue;
        }

        // 部屋のサイズと位置を調整
        float adjustedX = mapPos.x();
        float adjustedY = mapPos.y();
        float adjustedWidth = roomSize.x();
        float adjustedHeight = roomSize.y();

        // X方向の調整
        if (mapPos.x() - roomHalfWidth < m_position.x() - halfWidth) {
            const float overflow = (m_position.x() - halfWidth) - (mapPos.x() - roomHalfWidth);
            adjustedWidth -= overflow;
            adjustedX += overflow * 0.5f;
        }
        if (mapPos.x() + roomHalfWidth > m_position.x() + halfWidth) {
            const float overflow = (mapPos.x() + roomHalfWidth) - (m_position.x() + halfWidth);
            adjustedWidth -= overflow;
            adjustedX -= overflow * 0.5f;
        }

        // Y方向の調整
        if (mapPos.y() - roomHalfHeight < m_position.y() - halfHeight) {
            const float overflow = (m_position.y() - halfHeight) - (mapPos.y() - roomHalfHeight);
            adjustedHeight -= overflow;
            adjustedY += overflow * 0.5f;
        }
        if (mapPos.y() + roomHalfHeight > m_position.y() + halfHeight) {
            const float overflow = (mapPos.y() + roomHalfHeight) - (m_position.y() + halfHeight);
            adjustedHeight -= overflow;
            adjustedY -= overflow * 0.5f;
        }

        // 調整後のサイズと位置で部屋を描画
        renderer->drawRect(
            Vector2({ adjustedX, adjustedY }),
            Vector2({ adjustedWidth, adjustedHeight }),
            0.0f,
            Color({ 0.5f, 0.5f, 1.0f, 0.3f })); // 薄い青で半透明
    }
}

}