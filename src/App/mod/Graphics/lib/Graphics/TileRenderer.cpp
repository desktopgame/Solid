#include <Graphics/Camera.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/TileRenderer.hpp>

namespace Lib::Graphics {
// public
TileRenderer::TileRenderer(float tileSize)
    : m_tileSize(tileSize)
    , m_tileBatches()
{
}

int32_t TileRenderer::rentTile(TileBufferKind kind) { return getTileBatch(kind)->rent(); }

void TileRenderer::releaseTile(TileBufferKind kind, int32_t index) { getTileBatch(kind)->release(index); }

void TileRenderer::batchTileArray(TileBufferKind kind, int32_t index, const Math::Vector4* tiles, int32_t tileCount) { getTileBatch(kind)->setTiles(index, tiles, tileCount); }

void TileRenderer::batchTileMatrix(TileBufferKind kind, int32_t index, const Math::Matrix& matrix)
{
    getTileBatch(kind)->setMatrix(index, matrix);
}

void TileRenderer::drawTiles()
{
    for (auto tileBatch : m_tileBatches) {
        if (tileBatch) {
            tileBatch->setGlobalViewMatrix(Camera::getLookAtMatrix());
            tileBatch->setGlobalProjectionMatrix(Camera::getPerspectiveMatrix());
            Engine::getInstance()->getDevice()->getSurface()->render(tileBatch);
        }
    }
}
// private
std::shared_ptr<TileBatch> TileRenderer::getTileBatch(TileBufferKind kind)
{
    int32_t i = static_cast<int32_t>(kind);
    if (!m_tileBatches.at(i)) {
        switch (kind) {
        case TileBufferKind::UltraSmall:
            m_tileBatches.at(i) = TileBatch::create(TileBufferUltraSmall::create(100), m_tileSize);
            break;
        case TileBufferKind::Small:
            m_tileBatches.at(i) = TileBatch::create(TileBufferSmall::create(100), m_tileSize);
            break;
        case TileBufferKind::Medium:
            m_tileBatches.at(i) = TileBatch::create(TileBufferMedium::create(200), m_tileSize);
            break;
        case TileBufferKind::Large:
            m_tileBatches.at(i) = TileBatch::create(TileBufferLarge::create(100), m_tileSize);
            break;
        case TileBufferKind::UltraLarge:
            m_tileBatches.at(i) = TileBatch::create(TileBufferUltraLarge::create(100), m_tileSize);
            break;
        }
    }
    return m_tileBatches.at(i);
}

}