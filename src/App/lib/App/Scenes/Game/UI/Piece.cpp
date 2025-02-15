#include <App/Scenes/Game/UI/Piece.hpp>

namespace App::Scenes::Game::UI {
// Cell
Piece::Cell::Cell(const IntVector2& position)
    : position(position)
{
}
// public
Piece::Piece(const std::vector<Cell>& cells)
    : Component()
    , cells(cells)
    , m_minCellX()
    , m_maxCellX()
    , m_minCellY()
    , m_maxCellY()
    , m_cellCountX()
    , m_cellCountY()
{
    m_minCellX = 999;
    m_maxCellX = -999;
    m_minCellY = 999;
    m_maxCellY = -999;

    for (const auto& cell : cells) {
        if (cell.position.x() < m_minCellX) {
            m_minCellX = cell.position.x();
        }
        if (cell.position.x() > m_maxCellX) {
            m_maxCellX = cell.position.x();
        }
        if (cell.position.y() < m_minCellY) {
            m_minCellY = cell.position.y();
        }
        if (cell.position.y() > m_maxCellY) {
            m_maxCellY = cell.position.y();
        }
    }

    m_cellCountX = (m_maxCellX - m_minCellX) + 1;
    m_cellCountY = (m_maxCellY - m_minCellY) + 1;
    setPreferredSize(Vector2({ //
        (m_cellCountX * k_cellSize) + ((m_cellCountX + 1) * k_cellSpaceSize),
        (m_cellCountY * k_cellSize) + ((m_cellCountY + 1) * k_cellSpaceSize) }));
}

void Piece::update() { }
void Piece::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    auto center = getGlobalPosition();
    auto size = getSize();
    float left = center.x() - (size.x() / 2.0f);
    float top = center.y() + (size.y() / 2.0f);
    for (int32_t x = 0; x < m_cellCountX; x++) {
        for (int32_t y = 0; y < m_cellCountY; y++) {
            int32_t cellX = x + m_minCellX;
            int32_t cellY = y + m_minCellY;
            auto iter = std::find_if(cells.begin(), cells.end(), [cellX, cellY](const auto& cell) -> bool {
                return cellX == cell.position.x() && cellY == cell.position.y();
            });
            if (iter == cells.end()) {
                continue;
            }

            float spaceOffsetX = (x + 1) * k_cellSpaceSize;
            float spaceOffsetY = (y + 1) * k_cellSpaceSize;

            float cellOffsetX = (x * k_cellSize) + (k_cellSize / 2.0f);
            float cellOffsetY = (y * k_cellSize) + (k_cellSize / 2.0f);

            renderer->drawRect(
                Vector2({ //
                    left + spaceOffsetX + cellOffsetX,
                    top - spaceOffsetY - cellOffsetY }),
                Vector2({ //
                    k_cellSize,
                    k_cellSize }),
                0.0f, Vector4({ 0.5f, 0.5f, 0.5f, 1 }));
        }
    }
}
}