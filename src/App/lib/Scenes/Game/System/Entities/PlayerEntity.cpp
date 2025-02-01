#include <Common/Graphics/Node.hpp>
#include <Common/Graphics/NodeRegistry.hpp>
#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entities/ProjectileEntity.hpp>
#include <Scenes/Game/System/Field.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<PlayerEntity> PlayerEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<PlayerEntity>(new PlayerEntity(node));
}
PlayerEntity::~PlayerEntity() { }

void PlayerEntity::idle(const std::shared_ptr<Chunk>& chunk)
{
    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();

    Vector3 addVelocity;
    if (Cursor::isLocked()) {
        if (keyboard->isPressed(KeyCode::W)) {
            addVelocity += forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::S)) {
            addVelocity -= forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::D)) {
            addVelocity += right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::A)) {
            addVelocity -= right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        }

        bool isJump = false;
        if (keyboard->isPressed(KeyCode::Space)) {
            isJump = true;
        }

        if (isOnGround() && isJump) {
            setVelocity(Vector3({ 0, 30, 0 }));
        } else {
            if (addVelocity.length() > 0.0f) {
                setVelocity(getVelocity() * Vector3({ 0, 1, 0 }) + addVelocity);
            } else {
                setVelocity(getVelocity() * Vector3({ 0, 1, 0 }));
            }
        }

        auto mouseDelta = mouse->getDelta();
        m_cameraAngleX -= static_cast<float>(mouseDelta.y()) * m_cameraRotateSpeed;
        m_cameraAngleY += static_cast<float>(mouseDelta.x()) * m_cameraRotateSpeed;

        if (m_cameraAngleX <= -90.0f) {
            m_cameraAngleX = -89.0f;
        }
        if (m_cameraAngleX >= 90.0f) {
            m_cameraAngleX = 89.0f;
        }
    }

    if (keyboard->isTrigger(KeyCode::E)) {
        if (Cursor::isVisible()) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();
        }
    }

    if (mouse->isTrigger(Mouse::Button::Left)) {
        auto proj = ProjectileEntity::create(Common::Graphics::NodeRegistry::s_bulletNode->clone());
        proj->setOwner(shared_from_this());
        proj->setPosition(getPosition() + Vector3({ 0, 5, 0 }));
        proj->setRotation(Vector3({ m_cameraAngleX, Mathf::normalizeDegree(-m_cameraAngleY), 0.0f }));
        proj->setDirection(forward);
        proj->setSpeed(200);
        chunk->spwan(proj);
    }
}
void PlayerEntity::update(const std::shared_ptr<Chunk>& chunk)
{
    BasicEntity::update(chunk);

    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    Camera::position(getPosition());
    Camera::lookAt(getPosition() + forward);
    Camera::depthRange(0.1f, 1000.0f);
}
void PlayerEntity::draw3D(const std::shared_ptr<Renderer>& renderer) { }
void PlayerEntity::draw2D(const std::shared_ptr<Renderer>& renderer) { }
// protected
PlayerEntity::PlayerEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : BasicEntity(node)
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(40.0f)
    , m_cameraRotateSpeed(0.8f)
{
}
}