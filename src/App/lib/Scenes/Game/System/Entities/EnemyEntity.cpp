#include <Common/Graphics/ParticleSystem.hpp>
#include <Common/Graphics/SphericalParticle.hpp>
#include <Common/Graphics/TelopSystem.hpp>
#include <Scenes/Game/System/DamageSource.hpp>
#include <Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
EnemyEntity::~EnemyEntity() { }
// protected
EnemyEntity::EnemyEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : BasicEntity(node)
{
    m_category = Entity::Category::Enemy;
}

bool EnemyEntity::damage(const std::shared_ptr<DamageSource>& damageSource)
{
    bool applied = BasicEntity::damage(damageSource);

    if (applied) {
        std::string temp = std::to_string(damageSource->getPower());
        std::u16string text = std::u16string(temp.begin(), temp.end());
        Common::Graphics::TelopSystem::request(text, m_position, Vector3({ 1, 0, 0 }), 1.0f);
    }
    return applied;
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