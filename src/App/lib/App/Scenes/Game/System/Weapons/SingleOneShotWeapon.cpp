#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Scenes/Game/System/Chunk.hpp>
#include <App/Scenes/Game/System/Entities/ProjectileEntity.hpp>
#include <App/Scenes/Game/System/Weapons/SingleOneShotWeapon.hpp>
#include <App/library.hpp>

namespace App::Scenes::Game::System::Weapons {

SingleOneShotWeapon::SingleOneShotWeapon(const Weapon::Parameter& parameter, float energyMax)
    : Weapon(parameter)
    , m_energyMax(energyMax)
{
}
SingleOneShotWeapon::~SingleOneShotWeapon() { }

void SingleOneShotWeapon::execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player)
{
    auto proj = Entities::ProjectileEntity::create(Common::Graphics::NodeRegistry::s_bulletNode->clone(), Entity::Category::PlayerTeam);
    proj->setOwner(player);
    proj->setPosition(player->getPosition() + Vector3({ 0, 5, 0 }));
    proj->setRotation(Vector3({ player->getCameraAngleX(), Mathf::normalizeDegree(-player->getCameraAngleY()), 0.0f }));
    proj->setDirection(player->getForward());
    proj->setSpeed(200);
    chunk->spwan(proj);
}

Weapon::InputMethod SingleOneShotWeapon::getInputMethod() const { return Weapon::InputMethod::OneShot; }
float SingleOneShotWeapon::getEnergyMax() const { return m_energyMax; }

}