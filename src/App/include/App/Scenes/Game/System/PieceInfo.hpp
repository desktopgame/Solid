#pragma once
#include <App/Scenes/Game/System/Cell.hpp>
#include <App/library.hpp>

namespace App::Scenes::Game::System {
class PieceInfo {
public:
    explicit PieceInfo(const std::vector<Cell>& cells, const std::u16string& text);

    const std::vector<Cell> cells;
    std::u16string text;
};
}