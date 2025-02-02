#include <Scenes/Game/System/Minimap.hpp>

namespace App::Scenes::Game::System {

Minimap::Minimap(const std::shared_ptr<Field>& field)
    : m_field(field)
    , m_position(Vector2({ 150.0f, 150.0f })) // 左上から150pxの位置(中心座標)
    , m_size(Vector2({ 200.0f, 200.0f })) // 200x200のサイズ
{
}

void Minimap::update()
{
    // フィールドの更新に合わせて処理
    auto field = m_field.lock();
    if (!field) {
        return;
    }
}

void Minimap::drawEntity(
    const std::shared_ptr<Renderer>& renderer,
    const std::shared_ptr<Entity>& entity,
    const std::shared_ptr<Chunk>& chunk,
    const Color& color) const
{
    const auto& entityPos = entity->getPosition();
    // エンティティの位置がチャンクの範囲内かチェック
    if (entityPos.x() < chunk->getPhysicalMinX() || entityPos.x() > chunk->getPhysicalMaxX() || entityPos.z() < chunk->getPhysicalMinZ() || entityPos.z() > chunk->getPhysicalMaxZ()) {
        return; // チャンクの範囲外なのでスキップ
    }

    // エンティティの位置をミニマップ座標に変換
    const float normalizedX = (entityPos.x() - chunk->getPhysicalMinX()) / (chunk->getPhysicalMaxX() - chunk->getPhysicalMinX());
    const float normalizedZ = (entityPos.z() - chunk->getPhysicalMinZ()) / (chunk->getPhysicalMaxZ() - chunk->getPhysicalMinZ());

    // ミニマップの中心からのオフセットを計算
    const float offsetX = (normalizedX - 0.5f) * m_size.x();
    const float offsetZ = (normalizedZ - 0.5f) * m_size.y();

    Vector2 mapPos = Vector2({ m_position.x() + offsetX,
        m_position.y() + offsetZ });

    const float entitySize = 5.0f; // エンティティの表示サイズ

    // エンティティを描画
    renderer->drawRect(
        mapPos, // すでに中心位置
        Vector2({ entitySize, entitySize }),
        0.0f, // 回転なし
        color);
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

    // プレイヤーを描画
    auto player = field->getPlayer();
    if (player) {
        drawEntity(renderer, player, currentChunk, Color({ 0.0f, 1.0f, 0.0f, 1.0f })); // 緑色
    }

    // その他のエンティティを描画
    for (int32_t i = 0; i < currentChunk->getEntityCount(); ++i) {
        auto entity = currentChunk->getEntityAt(i);
        if (!entity) {
            continue;
        }
        drawEntity(renderer, entity, currentChunk, Color({ 1.0f, 0.0f, 0.0f, 1.0f })); // 赤色
    }
}

}