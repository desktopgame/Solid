#include <Common/Graphics/NodeRegistry.hpp>
#include <Scenes/Game/System/Entities/SlimeEntity.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<SlimeEntity> SlimeEntity::create()
{
    return std::shared_ptr<SlimeEntity>(new SlimeEntity());
}
SlimeEntity::~SlimeEntity()
{
}
void SlimeEntity::update(Field& field)
{
    switch (m_state) {
    case State::Wait:
        m_timer += Time::deltaTime();
        if (m_timer > 2.0f) {
            m_state = State::Walk;

            int32_t degree = m_random.range(0, 360);
            float x = Mathf::cos(Mathf::Deg2Rad * (float)degree);
            float z = Mathf::sin(Mathf::Deg2Rad * (float)degree);
            m_moveDir = Vector3({ x, 0, z });
            m_timer = 0.0f;
        }
        break;
    case State::Walk:
        m_timer += Time::deltaTime();
        if (m_timer < 3.0f) {
            setVelocity(m_moveDir * 20.0f);
        } else {
            m_state = State::Wait;
        }
        break;
    }

    BasicEntity::update(field);
}
// protected
SlimeEntity::SlimeEntity()
    : EnemyEntity(Common::Graphics::NodeRegistry::s_slimeNode->clone())
    , m_state()
    , m_timer()
    , m_moveDir()
    , m_random()
{
}

void SlimeEntity::onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z)
{
    if (m_state == State::Walk) {
        m_state = State::Wait;
        m_timer = 0;
    }
}
void SlimeEntity::onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z)
{
}
void SlimeEntity::onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z)
{
}
}