#pragma once
#include <App/library.hpp>

namespace App::Scenes::Game::System {
class Cell {
public:
    explicit Cell(const IntVector2& position);
    IntVector2 position;
};
}