#pragma once
#include <App/library.hpp>
#include <vector>

namespace App::Scenes::Game::UI {
class Piece : public Component {
public:
    class Cell {
    public:
        explicit Cell(const IntVector2& position);
        IntVector2 position;
    };

    explicit Piece(const std::vector<Cell>& cells);

    void update() override;
    void draw2D(const std::shared_ptr<Renderer>& renderer) override;

    const std::vector<Cell> cells;

private:
    static inline constexpr float k_cellSize = 10.0f;
    static inline constexpr float k_cellSpaceSize = 5.0f;

    int32_t m_minCellX = 999;
    int32_t m_maxCellX = -999;
    int32_t m_minCellY = 999;
    int32_t m_maxCellY = -999;
    int32_t m_cellCountX;
    int32_t m_cellCountY;
};
}