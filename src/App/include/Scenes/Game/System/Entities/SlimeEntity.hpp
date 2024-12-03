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

    virtual void onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z) override;

private:
    void stop();

    enum State : int32_t {
        Wait = 0,
        Look,
        Walk,
        Jump
    };

    State m_state;
    float m_timer;
    float m_waitTime;
    int32_t m_degree;
    bool m_jump;
    Vector3 m_moveDir;
    Random m_random;
};
}