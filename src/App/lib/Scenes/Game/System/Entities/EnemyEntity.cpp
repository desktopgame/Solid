#include <Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
EnemyEntity::~EnemyEntity() { }
// protected
EnemyEntity::EnemyEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : BasicEntity(node)
{
}
}