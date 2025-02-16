#include <App/Scenes/Game/System/PieceInstance.hpp>

namespace App::Scenes::Game::System {
PieceInstance::PieceInstance(const std::shared_ptr<PieceInfo>& info, const IntVector2& position)
    : m_info(info)
    , m_position(position)
{
}

std::shared_ptr<PieceInfo> PieceInstance::getInfo() const { return m_info; }
IntVector2 PieceInstance::getPosition() const { return m_position; }
}