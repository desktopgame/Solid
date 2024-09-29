#pragma once
#include <Graphics/TileBatch.hpp>
#include <Graphics/TileBuffer.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>

namespace Lib::Graphics {
class TileRenderer {
public:
    explicit TileRenderer(float tileSize);

    int32_t rentTile(TileBufferKind kind);
    void releaseTile(TileBufferKind kind, int32_t index);
    void batchTileArray(TileBufferKind kind, int32_t index, const Math::Vector4* tiles, int32_t tileCount);
    void batchTileMatrix(TileBufferKind kind, int32_t index, const Math::Matrix& matrix);
    void drawTiles();

private:
    std::shared_ptr<TileBatch> getTileBatch(TileBufferKind kind);

    float m_tileSize;
    std::array<std::shared_ptr<TileBatch>, 5> m_tileBatches;
};
}