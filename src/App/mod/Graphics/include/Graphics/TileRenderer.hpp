#pragma once
#include <Graphics/TileBatch.hpp>
#include <Graphics/TileBuffer.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <tuple>
#include <vector>

namespace Lib::Graphics {
class TileRenderer;
class Texture;
class TileTicket {
public:
    explicit TileTicket(
        TileRenderer& renderer,
        const std::vector<std::pair<TileBufferKind, int32_t>>& entries,
        int32_t tileCount);

    void batchTileArray(const Math::Vector4* tiles);
    void batchTileMatrix(const Math::Matrix& matrix);

    TileRenderer& renderer;
    std::vector<std::pair<TileBufferKind, int32_t>> entries;
    const int32_t tileCount;
};

class TileRenderer {
public:
    explicit TileRenderer(const std::shared_ptr<Texture>& normalTexture, float tileSize);

    int32_t rentTile(TileBufferKind kind);
    void releaseTile(TileBufferKind kind, int32_t index);

    std::shared_ptr<TileTicket> rentTileTicket(int32_t tileCount);
    void releaseTileTicket(const std::shared_ptr<TileTicket>& tileTicket);
    bool countSpace(int32_t tileCount, int32_t array[5]);

    void batchTileArray(TileBufferKind kind, int32_t index, const Math::Vector4* tiles, int32_t tileCount);
    void batchTileMatrix(TileBufferKind kind, int32_t index, const Math::Matrix& matrix);
    void drawTiles();

private:
    std::shared_ptr<TileBatch> getTileBatch(TileBufferKind kind);

    std::shared_ptr<Texture> m_normalTexture;
    float m_tileSize;
    std::array<std::shared_ptr<TileBatch>, 5> m_tileBatches;
};
}