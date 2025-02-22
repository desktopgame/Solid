#include <App/Scenes/Game/System/Weapons/SingleOneShotWeapon.hpp>

namespace App::Scenes::Game::System::Weapons {

SingleOneShotWeapon::SingleOneShotWeapon(const Weapon::Parameter& parameter, float energyMax)
    : Weapon(parameter)
    , m_energyMax(energyMax)
{
}
SingleOneShotWeapon::~SingleOneShotWeapon() { }

void SingleOneShotWeapon::execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player) { }

Weapon::InputMethod SingleOneShotWeapon::getInputMethod() const { return (Weapon::InputMethod)0; }
float SingleOneShotWeapon::getEnergyMax() const { return m_energyMax; }

}