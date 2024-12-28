#include <Scenes/Game/System/Entities/ProjectileEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<ProjectileEntity> ProjectileEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<ProjectileEntity>(new ProjectileEntity(node));
}
ProjectileEntity::~ProjectileEntity() { }

void ProjectileEntity::update(Field& field)
{
    setVelocity(m_direction * m_speed);
    BasicEntity::update(field);
}
void ProjectileEntity::onHitEnterEntity(const std::shared_ptr<Entity>& entity)
{
    auto owner = m_owner.lock();
    if (owner != nullptr && owner == entity) {
        return;
    }

    entity->damage(1);
    m_currentHP = 0;

    auto basicEntity = std::dynamic_pointer_cast<BasicEntity>(entity);
    if (basicEntity) {
        Vector3 vel = basicEntity->getVelocity();
        vel.x() = m_direction.x() * 20.0f;
        vel.z() = m_direction.z() * 20.0f;
        basicEntity->setVelocity(vel);
    }
}

void ProjectileEntity::setOwner(const std::weak_ptr<Entity>& owner) { m_owner = owner; }
std::shared_ptr<Entity> ProjectileEntity::getOwner() const { return m_owner.lock(); }

void ProjectileEntity::setDirection(const Vector3& direction) { m_direction = direction; }
Vector3 ProjectileEntity::getDirection() const { return m_direction; }

void ProjectileEntity::setSpeed(float speed) { m_speed = speed; }
float ProjectileEntity::getSpeed() const { return m_speed; }
// protected
ProjectileEntity::ProjectileEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : BasicEntity(node)
    , m_direction()
    , m_speed()
{
    setReceiveGravity(false);
}

void ProjectileEntity::onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z) { m_currentHP = 0; }
void ProjectileEntity::onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z) { m_currentHP = 0; }
void ProjectileEntity::onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z) { m_currentHP = 0; }

// private
}