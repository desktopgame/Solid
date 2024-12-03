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

int32_t Entity::getMaximumHP() const { return m_maximumHP; }
int32_t Entity::getCurrentHP() const { return m_currentHP; }
bool Entity::isDead() const { return m_currentHP == 0; }

void Entity::setPosition(const Vector3& position) { m_position = position; }
Vector3 Entity::getPosition() const { return m_position; }

void Entity::setRotation(const Vector3& rotation) { m_rotation = rotation; }
Vector3 Entity::getRotation() const { return m_rotation; }

// protected
Entity::Entity()
    : m_maximumHP(10)
    , m_currentHP(10)
    , m_position()
    , m_rotation()
{
}
}