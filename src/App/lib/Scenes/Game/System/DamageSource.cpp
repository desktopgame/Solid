#include <Scenes/Game/System/DamageSource.hpp>

namespace App::Scenes::Game::System {
// public
std::shared_ptr<DamageSource> DamageSource::create(const std::shared_ptr<Entity>& entity, int32_t power)
{
    auto damageSource = std::shared_ptr<DamageSource>(new DamageSource());
    damageSource->m_entity = entity;
    damageSource->m_power = power;
    return damageSource;
}

std::shared_ptr<Entity> DamageSource::getEntity() const { return m_entity; }
int32_t DamageSource::getPower() const { return m_power; }
// private
DamageSource::DamageSource()
    : m_entity()
    , m_power()
{
}
}