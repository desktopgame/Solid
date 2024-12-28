#pragma once
#include <library.hpp>
#include <memory>

namespace App::Scenes::Game::System {
class Field;
class Entity : public std::enable_shared_from_this<Entity> {
public:
    virtual ~Entity();

    virtual void update(Field& field) = 0;
    virtual void onGui() = 0;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) = 0;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) = 0;

    void addHitEntity(const std::shared_ptr<Entity>& entity);
    void removeHitEntity(const std::shared_ptr<Entity>& entity);
    bool isHitOnEntity(const std::shared_ptr<Entity>& entity) const;
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) = 0;
    virtual void onHitStayEntity(const std::shared_ptr<Entity>& entity) = 0;
    virtual void onHitExitEntity(const std::shared_ptr<Entity>& entity) = 0;

    void damage(int32_t power);

    virtual Geom::OBB getOBB() const;
    virtual Geom::AABB getAABB() const;

    int32_t getMaximumHP() const;
    int32_t getCurrentHP() const;
    bool isDead() const;

    void setPosition(const Vector3& position);
    Vector3 getPosition() const;

    void setRotation(const Vector3& rotation);
    Vector3 getRotation() const;

protected:
    Entity();

    virtual void onPositionChanged(const Vector3& position);
    virtual void onRotationChanged(const Vector3& rotation);

    std::vector<std::shared_ptr<Entity>> m_hitTable;
    int32_t m_maximumHP;
    int32_t m_currentHP;
    Vector3 m_position;
    Vector3 m_rotation;
};
}