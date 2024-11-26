#include <Common/FpsController.hpp>

namespace App::Common {
FpsController::FpsController()
    : m_position({ 0, 0, -3 })
    , m_lookAt({ 0, 0, 0 })
    , m_angleX()
    , m_angleY()
    , m_moveSpeed(0.08f)
    , m_rotateSpeed(0.8f)
{
}

void FpsController::update()
{
    auto rotation = Quaternion::angleAxis(m_angleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_angleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();
    if (Cursor::isLocked()) {
        if (keyboard->isPressed(KeyCode::W)) {
            m_position += forward * Vector3({ 1, 0, 1 }) * m_moveSpeed;
        } else if (keyboard->isPressed(KeyCode::S)) {
            m_position -= forward * Vector3({ 1, 0, 1 }) * m_moveSpeed;
        } else if (keyboard->isPressed(KeyCode::D)) {
            m_position += right * Vector3({ 1, 0, 1 }) * m_moveSpeed;
        } else if (keyboard->isPressed(KeyCode::A)) {
            m_position -= right * Vector3({ 1, 0, 1 }) * m_moveSpeed;
        }
        if (keyboard->isPressed(KeyCode::Space)) {
            m_position += Vector3({ 0, 1, 0 }) * m_moveSpeed;
        } else if (keyboard->isPressed(KeyCode::Shift)) {
            m_position -= Vector3({ 0, 1, 0 }) * m_moveSpeed;
        }
        m_lookAt = m_position + forward;

        auto mouseDelta = mouse->getDelta();
        m_angleX -= static_cast<float>(mouseDelta.y()) * m_rotateSpeed;
        m_angleY += static_cast<float>(mouseDelta.x()) * m_rotateSpeed;

        if (m_angleX <= -90.0f) {
            m_angleX = -89.0f;
        }
        if (m_angleX >= 90.0f) {
            m_angleX = 89.0f;
        }
    }

    if (keyboard->isTrigger(KeyCode::E)) {
        if (Cursor::isVisible()) {
            lockCursor();
        } else {
            unlockCursor();
        }
    }
}

void FpsController::lockCursor()
{
    Cursor::hide();
    Cursor::lock(Engine::getInstance()->getWindow());
}
void FpsController::unlockCursor()
{
    Cursor::show();
    Cursor::unlock();
}

void FpsController::setPosition(const Vector3& position) { m_position = position; }
Vector3 FpsController::getPosition() const { return m_position; }

void FpsController::setAngleX(float angleX) { m_angleX = angleX; }
float FpsController::getAngleX() const { return m_angleX; }

void FpsController::setAngleY(float angleY) { m_angleY = angleY; }
float FpsController::getAngleY() const { return m_angleY; }

Vector3 FpsController::getLookAt() const { return m_lookAt; }
Quaternion FpsController::getForward() const { return Quaternion::angleAxis(m_angleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_angleX, Vector3({ 1, 0, 0 })); }
Vector3 FpsController::getForwardDir() const { return Quaternion::transform(getForward(), Vector3({ 0, 0, 1 })); }

void FpsController::setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
float FpsController::getMoveSpeed() const { return m_moveSpeed; }
float& FpsController::getMoveSpeed() { return m_moveSpeed; }

void FpsController::setRotateSpeed(float rotateSpeed) { m_rotateSpeed = rotateSpeed; }
float FpsController::getRotateSpeed() const { return m_rotateSpeed; }
float& FpsController::getRotateSpeed() { return m_rotateSpeed; }
}