#pragma once
#include <App/Scenes/Game/System/PieceInfo.hpp>
#include <App/library.hpp>
#include <memory>

namespace App::Scenes::Game::System {
class PieceInstance {
public:
    explicit PieceInstance(const std::shared_ptr<PieceInfo>& info, const IntVector2& position);

    std::shared_ptr<PieceInfo> getInfo() const;
    IntVector2 getPosition() const;

private:
    std::shared_ptr<PieceInfo> m_info;
    IntVector2 m_position;
};
}