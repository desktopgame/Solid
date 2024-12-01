#include <Scenes/Game/System/Entities/SlimeEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<SlimeEntity> SlimeEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<SlimeEntity>(new SlimeEntity(node));
}
SlimeEntity::~SlimeEntity()
{
}
// protected
SlimeEntity::SlimeEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : EnemyEntity(node)
{
}
}