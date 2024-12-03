#pragma once
#include <library.hpp>
#include <memory>

namespace App::Scenes::Game::System {
class Field;
class Entity {
public:
    virtual ~Entity();

    virtual void update(Field& field) = 0;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) = 0;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) = 0;

    void damage(int32_t power);

    int32_t getMaximumHP() const;
    int32_t getCurrentHP() const;
    bool isDead() const;

    void setPosition(const Vector3& position);
    Vector3 getPosition() const;

    void setRotation(const Vector3& rotation);
    Vector3 getRotation() const;

protected:
    Entity();

    int32_t m_maximumHP;
    int32_t m_currentHP;
    Vector3 m_position;
    Vector3 m_rotation;
};
}