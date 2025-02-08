#include <Common/Graphics/ParticleSystem.hpp>
#include <Common/Graphics/SphericalParticle.hpp>
#include <Scenes/Game/System/DamageSource.hpp>
#include <Scenes/Game/System/Entities/ProjectileEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<ProjectileEntity> ProjectileEntity::create(const std::shared_ptr<Common::Graphics::Node>& node, Category category)
{
    return std::shared_ptr<ProjectileEntity>(new ProjectileEntity(node, category));
}
ProjectileEntity::~ProjectileEntity() { }

void ProjectileEntity::idle(const std::shared_ptr<Chunk>& chunk)
{
    setVelocity(m_direction * m_speed);
}
void ProjectileEntity::onHitEnterEntity(const std::shared_ptr<Entity>& entity)
{
    auto owner = m_owner.lock();
    if (owner != nullptr && owner == entity) {
        return;
    }

    entity->damage(DamageSource::create(shared_from_this(), 1));
    m_currentHP = 0;

    Common::Graphics::ParticleSystem::emit<Common::Graphics::SphericalParticle>(Common::Graphics::ParticleParameter<Common::Graphics::SphericalParticleOption>(
        Vector3({ 1.0f, 1.0f, 0.0f }),
        Vector3({ 0.1f, 0.1f, 0.1f }),
        0.25f,
        0.5f,
        Common::Graphics::SphericalParticleOption(m_position, 1.0f, 20.0f)));

    auto basicEntity = std::dynamic_pointer_cast<BasicEntity>(entity);
    if (basicEntity) {
        basicEntity->knockback(m_direction, 500.0f, 0.25f);
    }
}

void ProjectileEntity::setOwner(const std::weak_ptr<Entity>& owner) { m_owner = owner; }
std::shared_ptr<Entity> ProjectileEntity::getOwner() const { return m_owner.lock(); }

void ProjectileEntity::setDirection(const Vector3& direction) { m_direction = direction; }
Vector3 ProjectileEntity::getDirection() const { return m_direction; }

void ProjectileEntity::setSpeed(float speed) { m_speed = speed; }
float ProjectileEntity::getSpeed() const { return m_speed; }
// protected
ProjectileEntity::ProjectileEntity(const std::shared_ptr<Common::Graphics::Node>& node, Category category)
    : BasicEntity(node)
    , m_direction()
    , m_speed()
{
    setReceiveGravity(false);
    m_category = category;
}

void ProjectileEntity::onCollisionWall(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) { m_currentHP = 0; }
void ProjectileEntity::onCollisionRoof(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) { m_currentHP = 0; }
void ProjectileEntity::onCollisionFloor(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) { m_currentHP = 0; }

// private
}