#pragma once
#include <App/Scenes/Game/System/Chunk.hpp>
#include <App/Scenes/Game/System/ChunkGenerator.hpp>
#include <App/library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Game::System {
/**
 * ゲームの世界全体を表すフィールドです。
 */
class Field : public std::enable_shared_from_this<Field> {
public:
    explicit Field(
        const std::shared_ptr<Texture>& normalTexture,
        const std::shared_ptr<Texture>& borderTexture);

    /**
     * 最初のチャンクを生成します。
     */
    void generate();

    /**
     * フィールドを更新します。
     */
    void update();

    /**
     * フィールドのデバッグUIを描画します。
     */
    void onGui();

    /**
     * フィールドの3D要素を描画します。
     * @param renderer
     */
    void draw3D(const std::shared_ptr<Renderer>& renderer);

    /**
     * フィールドの2D要素を描画します。
     * @param renderer
     */
    void draw2D(const std::shared_ptr<Renderer>& renderer);

    /**
     * 指定の座標を含むチャンクを検索します。
     * @param outChunk
     * @param pos
     * @return
     */
    bool tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const Vector3& pos) const;

    /**
     * 指定のグリッド座標のチャンクを検索します。
     * @param outChunk
     * @param gridPosition
     * @return
     */
    bool tryFindChunk(std::optional<std::shared_ptr<Chunk>>& outChunk, const IntVector2& gridPosition) const;

    /**
     * 指定の座標をグリッド座標に変換し、その位置のチャンクをロードします。
     * @param pos
     * @return
     */
    std::shared_ptr<Chunk> loadChunk(const Vector3& pos);

    /**
     * 指定のグリッド座標のチャンクをロードします。
     * @param gridPosition
     * @return
     */
    std::shared_ptr<Chunk> loadChunk(const IntVector2& gridPosition);

    /**
     * 現在のチャンクの周囲のチャンクをロードします。
     */
    void reloadChunks();

    /**
     * 現在のチャンクを返します。
     * 現在のチャンクとは、プレイヤー座標を含むチャンクです。
     * @return
     */
    std::shared_ptr<Chunk> getCurrentChunk() const;

    /**
     * ロード済みチャンクを返します。
     * @return
     */
    std::vector<std::shared_ptr<Chunk>> getLoadedChunks() const;

    /**
     * 指定位置のチャンクを返します。
     * @param index
     * @return
     */
    std::shared_ptr<Chunk> getChunkAt(int32_t index) const;

    /**
     * チャンクの数を返します。
     * @return
     */
    int32_t getChunkCount() const;

    /**
     * グローバルなブロック座標をローカルなブロック座標に変換します。
     * @param x
     * @param y
     * @param z
     * @return
     */
    static inline IntVector3 toLocalBlockPosition(int32_t x, int32_t y, int32_t z)
    {
        int32_t gridPosX = x / Chunk::k_chunkSizeX;
        if (x < 0) {
            gridPosX--;
        }
        int32_t gridPosZ = z / Chunk::k_chunkSizeZ;
        if (z < 0) {
            gridPosZ--;
        }
        int32_t localX = x;
        localX -= gridPosX * Chunk::k_chunkSizeX;
        int32_t localZ = z;
        localZ -= gridPosZ * Chunk::k_chunkSizeZ;
        return IntVector3({ localX, y, localZ });
    }

    /**
     * ローカルなブロック座標をグローバルなブロック座標に変換します。
     * @param gridPosition
     * @param x
     * @param y
     * @param z
     * @return
     */
    static inline IntVector3 toGlobalBlockPosition(const IntVector2& gridPosition, int32_t x, int32_t y, int32_t z)
    {
        int32_t globalX = x;
        if (gridPosition.x() > 0) {
            globalX += gridPosition.x() * Chunk::k_chunkSizeX;
        } else if (gridPosition.x() < 0) {
            x = Chunk::k_chunkSizeX - x;
            globalX = (gridPosition.x() * Chunk::k_chunkSizeX) + x;
        }
        int32_t globalZ = z;
        if (gridPosition.y() > 0) {
            globalZ += gridPosition.y() * Chunk::k_chunkSizeZ;
        } else if (gridPosition.y() < 0) {
            z = Chunk::k_chunkSizeZ - z;
            globalZ = (gridPosition.y() * Chunk::k_chunkSizeZ) + z;
        }
        return IntVector3({ globalX, y, globalZ });
    }

    /**
     * 指定位置にブロックが存在するなら true を返します。
     * @param x
     * @param y
     * @param z
     * @return
     */
    inline bool hasBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        std::optional<std::shared_ptr<Chunk>> c;
        float fx = static_cast<float>(x) * Chunk::k_tileSize;
        float fy = static_cast<float>(y) * Chunk::k_tileSize;
        float fz = static_cast<float>(z) * Chunk::k_tileSize;
        if (tryFindChunk(c, Vector3({ fx, fy, fz }))) {
            IntVector2 gridPos = (*c)->getGridPosition();
            x -= gridPos.x() * Chunk::k_chunkSizeX;
            z -= gridPos.y() * Chunk::k_chunkSizeZ;
            return (*c)->hasBlockAt(x, y, z);
        }
        return false;
    }

    /**
     * 指定位置のブロック情報を返します。
     * @param x
     * @param y
     * @param z
     * @return
     */
    inline int32_t getBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        std::optional<std::shared_ptr<Chunk>> c;
        float fx = static_cast<float>(x) * Chunk::k_tileSize;
        float fy = static_cast<float>(y) * Chunk::k_tileSize;
        float fz = static_cast<float>(z) * Chunk::k_tileSize;
        if (tryFindChunk(c, Vector3({ fx, fy, fz }))) {
            IntVector2 gridPos = (*c)->getGridPosition();
            x -= gridPos.x() * Chunk::k_chunkSizeX;
            z -= gridPos.y() * Chunk::k_chunkSizeZ;
            return (*c)->getBlockAt(x, y, z);
        }
        return 0;
    }

    /**
     * プレイヤーを設定します。
     * @param player
     */
    void setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player);

    /**
     * プレイヤーを返します。
     * @return
     */
    std::shared_ptr<Entities::PlayerEntity> getPlayer() const;

private:
    std::shared_ptr<Texture> m_normalTexture;
    std::shared_ptr<Texture> m_borderTexture;
    std::vector<std::shared_ptr<Chunk>> m_chunks;
    std::vector<std::shared_ptr<Chunk>> m_loadedChunks;
    std::shared_ptr<Entities::PlayerEntity> m_player;
};
}