#include <App/Scenes/Game/System/WeaponRegistry.hpp>
#include <App/Scenes/Game/System/Weapons/SingleOneShotWeapon.hpp>

namespace App::Scenes::Game::System {
std::shared_ptr<Weapon> WeaponRegistry::s_singleOneShotWeaponLv1;
void WeaponRegistry::initialize()
{
    {
        Weapon::Parameter parameter;
        parameter.power = 1;
        parameter.fireRate = 0.25f;
        parameter.coolRate = 0.5f;
        parameter.increaseEnergy = 1.0f;
        parameter.decreaseEnergy = 2.0f;
        s_singleOneShotWeaponLv1 = std::make_shared<Weapons::SingleOneShotWeapon>(parameter, 10.0f);
    }
}

void WeaponRegistry::destroy()
{
    s_singleOneShotWeaponLv1 = nullptr;
}
}