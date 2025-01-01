#include <Common/Graphics/ParticleSystem.hpp>
#include <Common/Graphics/SphericalParticle.hpp>
#include <Common/Graphics/TelopSystem.hpp>
#include <Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
EnemyEntity::~EnemyEntity() { }
// protected
EnemyEntity::EnemyEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : BasicEntity(node)
{
}

void EnemyEntity::damage(const std::shared_ptr<DamageSource>& damageSource)
{
    BasicEntity::damage(damageSource);
    Common::Graphics::TelopSystem::request(u"A", m_position, Vector3({ 1, 1, 1 }), 10.0f);
}

void EnemyEntity::onDead(const std::shared_ptr<DamageSource>& damageSource)
{
    Common::Graphics::ParticleSystem::emit<Common::Graphics::SphericalParticle>(Common::Graphics::ParticleParameter<Common::Graphics::SphericalParticleOption>(
        Vector3({ 1.0f, 1.0f, 1.0f }),
        Vector3({ 0.25f, 0.25f, 0.25f }),
        0.5f,
        1.0f,
        Common::Graphics::SphericalParticleOption(m_position, 1.0f, 20.0f)));
}
}