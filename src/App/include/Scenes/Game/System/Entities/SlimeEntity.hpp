#pragma once
#include <Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class SlimeEntity : public EnemyEntity {
public:
    static std::shared_ptr<SlimeEntity> create();
    virtual ~SlimeEntity();

    virtual void update(Field& field) override;

protected:
    SlimeEntity();

    virtual void onCollisionWall(int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionRoof(int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionFloor(int32_t x, int32_t y, int32_t z) override;

private:
    enum State : int32_t {
        Wait = 0,
        Walk,
        Jump
    };

    State m_state;
    float m_timer;
    Vector3 m_moveDir;
    Random m_random;
};
}