#include <Common/Graphics/NodeRegistry.hpp>
#include <Scenes/Game/System/Entities/SlimeEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<SlimeEntity> SlimeEntity::create()
{
    return std::shared_ptr<SlimeEntity>(new SlimeEntity());
}
SlimeEntity::~SlimeEntity()
{
}
// protected
SlimeEntity::SlimeEntity()
    : EnemyEntity(Common::Graphics::NodeRegistry::s_slimeNode->clone())
{
}
}