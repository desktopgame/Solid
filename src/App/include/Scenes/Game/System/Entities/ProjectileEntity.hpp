#pragma once
#include <Scenes/Game/System/Entities/BasicEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class ProjectileEntity : public BasicEntity {
public:
    static std::shared_ptr<ProjectileEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~ProjectileEntity();

    virtual void idle(Field& field) override;
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) override;

    void setOwner(const std::weak_ptr<Entity>& owner);
    std::shared_ptr<Entity> getOwner() const;

    void setDirection(const Vector3& direction);
    Vector3 getDirection() const;

    void setSpeed(float speed);
    float getSpeed() const;

protected:
    ProjectileEntity(const std::shared_ptr<Common::Graphics::Node>& node);

    virtual void onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z) override;

    std::weak_ptr<Entity> m_owner;
    Vector3 m_direction;
    float m_speed;

private:
};
}