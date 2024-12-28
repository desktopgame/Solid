#include <Scenes/Game/System/Entity.hpp>

namespace App::Scenes::Game::System {
// public
Entity::~Entity() { }

void Entity::damage(int32_t power)
{
    int32_t hp = m_currentHP - power;
    if (hp < 0) {
        hp = 0;
    }
    m_currentHP = hp;
}

Geom::OBB Entity::getOBB() const
{
    Geom::OBB obb;
    obb.axes = std::array<Vector3, 3> {
        Vector3({ 1, 0, 0 }),
        Vector3({ 0, 1, 0 }),
        Vector3({ 0, 0, 1 })
    };
    obb.center = Vector3({ 0, 0, 0 });
    obb.size = Vector3({ 0, 0, 0 });
    return obb;
}
Geom::AABB Entity::getAABB() const
{
    Geom::AABB aabb;
    aabb.min = Vector3({ 0, 0, 0 });
    aabb.max = Vector3({ 0, 0, 0 });
    return aabb;
}

int32_t Entity::getMaximumHP() const { return m_maximumHP; }
int32_t Entity::getCurrentHP() const { return m_currentHP; }
bool Entity::isDead() const { return m_currentHP == 0; }

void Entity::setPosition(const Vector3& position)
{
    m_position = position;
    onPositionChanged(position);
}
Vector3 Entity::getPosition() const { return m_position; }

void Entity::setRotation(const Vector3& rotation)
{
    m_rotation = rotation;
    onRotationChanged(rotation);
}
Vector3 Entity::getRotation() const { return m_rotation; }

// protected
Entity::Entity()
    : m_maximumHP(10)
    , m_currentHP(10)
    , m_position()
    , m_rotation()
{
}
void Entity::onPositionChanged(const Vector3& position) { }
void Entity::onRotationChanged(const Vector3& rotation) { }
}