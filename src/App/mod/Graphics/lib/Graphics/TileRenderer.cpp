#include <Graphics/Camera.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/TileRenderer.hpp>
#include <Math/Mathf.hpp>
#include <cassert>

namespace Lib::Graphics {
// TileTicket
TileTicket::TileTicket(
    TileRenderer& renderer,
    const std::vector<std::pair<TileBufferKind, int32_t>>& entries,
    int32_t tileCount)
    : renderer(renderer)
    , entries(entries)
    , tileCount(tileCount)
{
}

void TileTicket::batchTileArray(const Math::Vector4* tiles)
{
    int32_t count = tileCount;
    int32_t offset = 0;
    for (const auto& entry : entries) {
        int32_t space = 0;
        switch (entry.first) {
        case TileBufferKind::UltraSmall:
            space = TileBufferUltraSmall::ArraySize;
            break;
        case TileBufferKind::Small:
            space = TileBufferSmall::ArraySize;
            break;
        case TileBufferKind::Medium:
            space = TileBufferMedium::ArraySize;
            break;
        case TileBufferKind::Large:
            space = TileBufferLarge::ArraySize;
            break;
        case TileBufferKind::UltraLarge:
            space = TileBufferUltraLarge::ArraySize;
            break;
        }

        if ((count - space) >= 0) {
            renderer.batchTileArray(entry.first, entry.second, tiles + offset, space);
            count -= space;
            offset += space;
        } else {
            assert(count > 0);
            renderer.batchTileArray(entry.first, entry.second, tiles + offset, count);
            offset = tileCount;
            count = 0;
        }
    }
}

void TileTicket::batchTileMatrix(const Math::Matrix& matrix)
{
    for (const auto& entry : entries) {
        renderer.batchTileMatrix(entry.first, entry.second, matrix);
    }
}
// public
TileRenderer::TileRenderer(float tileSize)
    : m_tileSize(tileSize)
    , m_tileBatches()
{
}

int32_t TileRenderer::rentTile(TileBufferKind kind) { return getTileBatch(kind)->rent(); }

void TileRenderer::releaseTile(TileBufferKind kind, int32_t index) { getTileBatch(kind)->release(index); }

std::shared_ptr<TileTicket> TileRenderer::rentTileTicket(int32_t tileCount)
{
    std::array<int32_t, 5> spaceTable;
    if (!countSpace(tileCount, spaceTable.data())) {
        return nullptr;
    }
    std::vector<std::pair<TileBufferKind, int32_t>> entries;
    for (int32_t i = 0; i < 5; i++) {
        TileBufferKind kind = static_cast<TileBufferKind>(i);
        for (int32_t j = 0; j < spaceTable.at(i); j++) {
            int32_t id = rentTile(kind);
            entries.push_back({ kind, id });
        }
    }
    return std::make_shared<TileTicket>(*this, entries, tileCount);
}

void TileRenderer::releaseTileTicket(const std::shared_ptr<TileTicket>& tileTicket)
{
    for (const auto& entry : tileTicket->entries) {
        releaseTile(entry.first, entry.second);
    }
}

bool TileRenderer::countSpace(int32_t tileCount, int32_t array[5])
{
    std::array<int32_t, 5> spaceTable;
    std::fill(spaceTable.begin(), spaceTable.end(), 0);
    for (int32_t i = 0; i < 5; i++) {
        TileBufferKind kind = static_cast<TileBufferKind>(i);
        spaceTable.at(i) = getTileBatch(kind)->countSpace();
    }

    if (array != nullptr) {
        for (int32_t i = 0; i < 5; i++) {
            array[i] = 0;
        }
    }

    for (int32_t i = 4; i >= 0; i--) {
        TileBufferKind kind = static_cast<TileBufferKind>(i);
        int32_t space = 0;
        switch (kind) {
        case TileBufferKind::UltraSmall:
            space = TileBufferUltraSmall::ArraySize;
            break;
        case TileBufferKind::Small:
            space = TileBufferSmall::ArraySize;
            break;
        case TileBufferKind::Medium:
            space = TileBufferMedium::ArraySize;
            break;
        case TileBufferKind::Large:
            space = TileBufferLarge::ArraySize;
            break;
        case TileBufferKind::UltraLarge:
            space = TileBufferUltraLarge::ArraySize;
            break;
        }

        while (tileCount > space) {
            if (spaceTable.at(i) == 0) {
                break;
            }
            if (tileCount <= 0) {
                break;
            }
            tileCount -= space;
            spaceTable.at(i) -= 1;

            if (array != nullptr) {
                array[i] += 1;
            }
        }
    }

    for (int32_t i = 0; i < 5; i++) {
        TileBufferKind kind = static_cast<TileBufferKind>(i);
        int32_t space = 0;
        switch (kind) {
        case TileBufferKind::UltraSmall:
            space = TileBufferUltraSmall::ArraySize;
            break;
        case TileBufferKind::Small:
            space = TileBufferSmall::ArraySize;
            break;
        case TileBufferKind::Medium:
            space = TileBufferMedium::ArraySize;
            break;
        case TileBufferKind::Large:
            space = TileBufferLarge::ArraySize;
            break;
        case TileBufferKind::UltraLarge:
            space = TileBufferUltraLarge::ArraySize;
            break;
        }

        while (tileCount < space) {
            if (spaceTable.at(i) == 0) {
                break;
            }
            if (tileCount <= 0) {
                break;
            }
            tileCount -= space;
            spaceTable.at(i) -= 1;

            if (array != nullptr) {
                array[i] += 1;
            }
        }
    }
    return tileCount <= 0;
}

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
        auto tex = Texture::create("./assets/tileNormal2.png");
        switch (kind) {
        case TileBufferKind::UltraSmall:
            m_tileBatches.at(i) = TileBatch::create(TileBufferUltraSmall::create(100), tex, m_tileSize);
            break;
        case TileBufferKind::Small:
            m_tileBatches.at(i) = TileBatch::create(TileBufferSmall::create(100), tex, m_tileSize);
            break;
        case TileBufferKind::Medium:
            m_tileBatches.at(i) = TileBatch::create(TileBufferMedium::create(200), tex, m_tileSize);
            break;
        case TileBufferKind::Large:
            m_tileBatches.at(i) = TileBatch::create(TileBufferLarge::create(100), tex, m_tileSize);
            break;
        case TileBufferKind::UltraLarge:
            m_tileBatches.at(i) = TileBatch::create(TileBufferUltraLarge::create(100), tex, m_tileSize);
            break;
        }
    }
    return m_tileBatches.at(i);
}

}