#pragma once
#include <App/library.hpp>
#include <vector>

namespace App::Scenes::Game::System {
/**
 * チャンクを構成するブロックを生成するクラスです。
 */
class ChunkGenerator {
public:
    explicit ChunkGenerator();

    /**
     * ブロックを生成します。
     */
    void generate();

    /**
     * ブロック情報を返します。
     */
    const std::vector<Vector4>& getTiles() const;

private:
    std::vector<Vector4> m_tiles;
};
}