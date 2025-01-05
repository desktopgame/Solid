#include <Common/Graphics/NodeRegistry.hpp>
#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <imgui.h>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<SlimeEntity> SlimeEntity::create()
{
    return std::shared_ptr<SlimeEntity>(new SlimeEntity());
}
SlimeEntity::~SlimeEntity()
{
}
void SlimeEntity::idle(Field& field)
{
    switch (m_state) {
    case State::Wait: {
        if (isOnGround()) {
            m_timer += Time::deltaTime();
        }
        if (m_timer > m_waitTime) {
            m_state = State::Look;

            int32_t degree = m_random.range(0, 360);
            Vector3 playerPos = field.getPlayer()->getPosition();
            Vector3 selfPos = getPosition();
            float distToPlayer = Vector3::distance(playerPos, selfPos);
            if (distToPlayer < 30.0f) {
                Vector3 dir = Vector3::normalized(playerPos - selfPos);
                degree = (::atan2f(dir.z(), dir.x()) * Mathf::Rad2Deg);
            }

            float x = Mathf::cos(Mathf::Deg2Rad * (float)degree);
            float z = Mathf::sin(Mathf::Deg2Rad * (float)degree);
            m_degree = degree;
            m_moveDir = Vector3({ x, 0, z });
            m_timer = 0.0f;
        }
        break;
    }
    case State::Look: {
        float targetDegree = Mathf::normalizeDegree(m_degree + 90.0f);
        float diff = Mathf::diffDegree(m_rotation.y(), targetDegree);
        if (Mathf::abs(diff) > 5) {
            setTorque(Vector3({ 0, 200 * Mathf::sign(diff), 0 }));
        } else {
            m_state = State::Walk;
            setTorque(Vector3({ 0, 0, 0 }));

            m_jump = false;
            m_moveSpeed = 30.0f;
            if (m_random.range(0, 10) > 5) {
                setVelocity(Vector3({ 0, 30, 0 }));
                m_jump = true;
                m_moveSpeed = 20.0f;
            }
        }
        break;
    }
    case State::Walk:
    case State::Back: {
        m_timer += Time::deltaTime();
        if (m_timer < 2.0f) {
            Vector3 vel = getVelocity() * Vector3({ 0, 1, 0 });
            setVelocity(vel + (m_moveDir * m_moveSpeed));

            if (m_jump && isOnGround()) {
                stop();
            }
        } else {
            stop();
        }
        break;
    }
    case State::Jump: {
        break;
    }
    }
}
void SlimeEntity::onGui()
{
    switch (m_state) {
    case State::Wait:
        ImGui::LabelText("State", "Wait");
        break;
    case State::Look:
        ImGui::LabelText("State", "Look");
        break;
    case State::Walk:
        ImGui::LabelText("State", "Walk");
        break;
    case State::Back:
        ImGui::LabelText("State", "Back");
        break;
    default:
        break;
    }
}
// protected
SlimeEntity::SlimeEntity()
    : EnemyEntity(Common::Graphics::NodeRegistry::s_slimeNode->clone())
    , m_state()
    , m_timer()
    , m_waitTime(2.0f)
    , m_moveSpeed()
    , m_degree()
    , m_jump()
    , m_moveDir()
    , m_random()
{
    m_currentHP = m_maximumHP = 3;
}

void SlimeEntity::onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z)
{
    if (m_state == State::Walk) {
        stop();
    } else if (m_state == State::Back) {
        m_moveDir = -m_moveDir;
    }
}
void SlimeEntity::onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z)
{
}
void SlimeEntity::onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z)
{
}
void SlimeEntity::onRotationStop(Field& field)
{
    if (m_state == State::Look) {
        m_state = State::Back;
        float vx = Mathf::cos(Mathf::normalizeDegree(m_rotation.y() - 90.0f) * Mathf::Deg2Rad);
        float vz = Mathf::sin(Mathf::normalizeDegree(m_rotation.y() - 90.0f) * Mathf::Deg2Rad);
        m_moveDir = -Vector3({ vx, 0, vz });
        m_timer = 0.0f;
        m_jump = false;
        setTorque(Vector3({ 0, 0, 0 }));
    }
}
// private
void SlimeEntity::stop()
{
    m_state = State::Wait;
    m_timer = 0.0f;
    m_waitTime = m_random.range(1.0f, 3.0f);

    Vector3 vel = getVelocity();
    vel.x() = 0.0f;
    vel.z() = 0.0f;
    setVelocity(vel);
}
}