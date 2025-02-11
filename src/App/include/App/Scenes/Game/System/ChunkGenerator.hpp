#pragma once
#include <App/library.hpp>
#include <vector>

namespace App::Scenes::Game::System {
class ChunkGenerator {
public:
    explicit ChunkGenerator();
    void generate();

    const std::vector<Vector4>& getTiles() const;

private:
    std::vector<Vector4> m_tiles;
};
}