#pragma once
#include <memory>

namespace App::Scenes::Game::System::Entities {
class PlayerEntity;
}

namespace App::Scenes::Game::System {
class Field;
class Chunk;
class Weapon {
public:
    class Parameter {
    public:
        int32_t power;
        float fireRate;
        float coolRate;
        float increaseEnergy;
        float decreaseEnergy;
    };

    enum class InputMethod {
        None,
        LeftOneShot,
        LeftContinuous,
        RightOneShot,
        RightContinuous
    };

    explicit Weapon(const Parameter& parameter);
    virtual ~Weapon();

    virtual void execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player) = 0;

    virtual InputMethod getInputMethod() const = 0;
    virtual float getEnergyMax() const = 0;

    const Parameter parameter;
};
}