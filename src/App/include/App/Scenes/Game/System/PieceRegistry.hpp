#pragma once
#include <App/Scenes/Game/System/PieceInfo.hpp>
#include <memory>
#include <vector>

namespace App::Scenes::Game::System {
class PieceRegistry {
public:
    static void initialize();
    static void destroy();

    static std::vector<std::shared_ptr<PieceInfo>> s_pieces;

private:
};
}