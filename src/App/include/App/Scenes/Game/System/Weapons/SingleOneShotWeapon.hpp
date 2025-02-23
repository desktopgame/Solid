#pragma once
#include <App/Scenes/Game/System/Weapon.hpp>

namespace App::Scenes::Game::System::Weapons {
class SingleOneShotWeapon : public Weapon {
public:
    explicit SingleOneShotWeapon(const Weapon::Parameter& parameter, float energyMax);
    ~SingleOneShotWeapon();

    void execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player) override;

    Weapon::InputMethod getInputMethod() const override;
    float getEnergyMax() const override;
    std::string getIconPath() const override;

private:
    float m_energyMax;
};
}