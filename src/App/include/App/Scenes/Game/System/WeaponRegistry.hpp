#pragma once
#include <App/Scenes/Game/System/Weapon.hpp>
#include <memory>
#include <vector>

namespace App::Scenes::Game::System {
class WeaponRegistry {
public:
    static void initialize();
    static void destroy();

    static std::shared_ptr<Weapon> s_singleOneShotWeaponLv1;
};
}