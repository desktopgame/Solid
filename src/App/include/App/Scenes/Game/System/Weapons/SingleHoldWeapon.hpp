#pragma once
#include <App/Scenes/Game/System/Weapon.hpp>

namespace App::Scenes::Game::System::Weapons {
class SingleHoldWeapon : public Weapon {
public:
    explicit SingleHoldWeapon(const Weapon::Parameter& parameter, float energyMax);
    ~SingleHoldWeapon();

    void execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player) override;

    Weapon::InputMethod getInputMethod() const override;
    float getEnergyMax() const override;

private:
    float m_energyMax;
};
}