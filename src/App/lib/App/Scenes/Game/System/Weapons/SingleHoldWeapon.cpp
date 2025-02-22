#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Scenes/Game/System/Chunk.hpp>
#include <App/Scenes/Game/System/Entities/ProjectileEntity.hpp>
#include <App/Scenes/Game/System/Weapons/SingleHoldWeapon.hpp>
#include <App/library.hpp>

namespace App::Scenes::Game::System::Weapons {

SingleHoldWeapon::SingleHoldWeapon(const Weapon::Parameter& parameter, float energyMax)
    : Weapon(parameter)
    , m_energyMax(energyMax)
{
}
SingleHoldWeapon::~SingleHoldWeapon() { }

void SingleHoldWeapon::execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player)
{
    auto proj = Entities::ProjectileEntity::create(Common::Graphics::NodeRegistry::s_bulletNode->clone(), Entity::Category::PlayerTeam);
    proj->setOwnerCategory(Entity::Category::PlayerTeam);
    proj->setPosition(player->getPosition() + Vector3({ 0, 5, 0 }));
    proj->setRotation(Vector3({ player->getCameraAngleX(), Mathf::normalizeDegree(-player->getCameraAngleY()), 0.0f }));
    proj->setDirection(player->getForward());
    proj->setSpeed(200);
    chunk->spwan(proj);
}

Weapon::InputMethod SingleHoldWeapon::getInputMethod() const { return Weapon::InputMethod::Hold; }
float SingleHoldWeapon::getEnergyMax() const { return m_energyMax; }
std::string SingleHoldWeapon::getIconPath() const { return "./assets/Textures/Machinegun.png"; }
}