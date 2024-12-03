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
        if (isOnGround()) {
            m_timer += Time::deltaTime();
        }
        if (m_timer > m_waitTime) {
            m_state = State::Look;

            int32_t degree = m_random.range(0, 360);
            float x = Mathf::cos(Mathf::Deg2Rad * (float)degree);
            float z = Mathf::sin(Mathf::Deg2Rad * (float)degree);
            m_degree = degree;
            m_moveDir = Vector3({ x, 0, z });
            m_timer = 0.0f;
        }
        break;
    case State::Look:
        if (Mathf::abs(m_rotation.y() - Mathf::normalizeDegree(m_degree + 90.0f)) > 5) {
            setTorque(Vector3({ 0, 120, 0 }));
        } else {
            m_state = State::Walk;
            setTorque(Vector3({ 0, 0, 0 }));

            if (m_random.range(0, 10) > 5) {
                setVelocity(Vector3({ 0, 30, 0 }));
            }
        }
        break;
    case State::Walk:
        m_timer += Time::deltaTime();
        if (m_timer < 3.0f) {
            Vector3 vel = getVelocity() * Vector3({ 0, 1, 0 });
            setVelocity(vel + (m_moveDir * 20.0f));
        } else {
            stop();
        }
        break;
    case State::Jump:
        break;
    }

    BasicEntity::update(field);
}
// protected
SlimeEntity::SlimeEntity()
    : EnemyEntity(Common::Graphics::NodeRegistry::s_slimeNode->clone())
    , m_state()
    , m_timer()
    , m_waitTime(2.0f)
    , m_degree()
    , m_moveDir()
    , m_random()
{
}

void SlimeEntity::onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z)
{
    if (m_state == State::Walk) {
        stop();
    }
}
void SlimeEntity::onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z)
{
}
void SlimeEntity::onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z)
{
}
// private
void SlimeEntity::stop()
{
    m_state = State::Wait;
    m_waitTime = m_random.range(1.0f, 3.0f);

    Vector3 vel = getVelocity();
    vel.x() = 0.0f;
    vel.z() = 0.0f;
    setVelocity(vel);
}
}