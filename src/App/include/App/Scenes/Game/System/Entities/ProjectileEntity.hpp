#pragma once
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class ProjectileEntity : public BasicEntity {
public:
    static std::shared_ptr<ProjectileEntity> create(const std::shared_ptr<Common::Graphics::Node>& node, Category category);
    virtual ~ProjectileEntity();

    virtual void idle(const std::shared_ptr<Chunk>& chunk) override;
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) override;

    void setOwnerCategory(Entity::Category ownerCategory);
    Entity::Category getOwnerCategory() const;

    void setDirection(const Vector3& direction);
    Vector3 getDirection() const;

    void setSpeed(float speed);
    float getSpeed() const;

protected:
    ProjectileEntity(const std::shared_ptr<Common::Graphics::Node>& node, Category category);

    virtual void onCollisionWall(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionRoof(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionFloor(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) override;

    Entity::Category m_ownerCategory;
    Vector3 m_direction;
    float m_speed;

private:
};
}