#pragma once
#include <App/Scenes/Game/System/Cell.hpp>
#include <App/library.hpp>
#include <vector>

namespace App::Scenes::Game::UI {
class Piece : public Component {
public:
    explicit Piece();

    void update() override;
    void draw2D(const std::shared_ptr<Renderer>& renderer) override;

    void setCells(const std::vector<System::Cell>& cells);
    const std::vector<System::Cell>& getCells() const;

private:
    static inline constexpr float k_cellSize = 10.0f;
    static inline constexpr float k_cellSpaceSize = 5.0f;

    int32_t m_minCellX = 999;
    int32_t m_maxCellX = -999;
    int32_t m_minCellY = 999;
    int32_t m_maxCellY = -999;
    int32_t m_cellCountX;
    int32_t m_cellCountY;
    std::vector<System::Cell> m_cells;
};
}