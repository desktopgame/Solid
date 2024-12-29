#pragma once
#include <memory>

namespace App::Scenes::Game::System {
class Entity;
class DamageSource {
public:
    static std::shared_ptr<DamageSource> create(const std::shared_ptr<Entity>& entity, int32_t power);

    std::shared_ptr<Entity> getEntity() const;
    int32_t getPower() const;

private:
    DamageSource();

    std::shared_ptr<Entity> m_entity;
    int32_t m_power;
};
}