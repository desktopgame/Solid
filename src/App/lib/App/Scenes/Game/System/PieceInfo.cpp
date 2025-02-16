#include <App/Scenes/Game/System/PieceInfo.hpp>

namespace App::Scenes::Game::System {
PieceInfo::PieceInfo(const std::vector<Cell>& cells, const std::u16string& text)
    : cells(cells)
    , text(text)
{
}
}