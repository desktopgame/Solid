#include <App/Scenes/Game/System/PieceInfo.hpp>

namespace App::Scenes::Game::System {
PieceInfo::PieceInfo(const std::vector<Cell>& cells, const std::u16string& text)
    : m_cells(cells)
    , m_text(text)
{
}

const std::vector<Cell>& PieceInfo::getCells() const { return m_cells; }
std::u16string PieceInfo::getText() const { return m_text; }
}