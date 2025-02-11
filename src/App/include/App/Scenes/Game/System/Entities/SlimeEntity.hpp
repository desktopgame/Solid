#pragma once
#include <App/Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class SlimeEntity : public EnemyEntity {
public:
    static std::shared_ptr<SlimeEntity> create();
    virtual ~SlimeEntity();

    virtual void idle(const std::shared_ptr<Chunk>& chunk) override;
    virtual void onGui() override;

protected:
    SlimeEntity();

    virtual void onCollisionWall(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionRoof(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) override;
    virtual void onCollisionFloor(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z) override;
    virtual void onRotationStop(const std::shared_ptr<Chunk>& chunk) override;

private:
    void stop();

    enum State : int32_t {
        Wait = 0,
        Look,
        Walk,
        Back,
        Jump
    };

    State m_state;
    float m_timer;
    float m_waitTime;
    float m_moveSpeed;
    int32_t m_degree;
    bool m_jump;
    Vector3 m_moveDir;
    Random m_random;
};
}