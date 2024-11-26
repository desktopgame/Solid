#include <Common/FpsController.hpp>

namespace App::Common {
FpsController::FpsController()
    : m_cameraPos({ 0, 0, -3 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.08f)
    , m_cameraRotateSpeed(0.8f)
    , m_cursorVisible(true)
{
}

void FpsController::update()
{
    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();
    if (Cursor::isLocked()) {
        if (keyboard->isPressed(KeyCode::W)) {
            m_cameraPos += forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::S)) {
            m_cameraPos -= forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::D)) {
            m_cameraPos += right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::A)) {
            m_cameraPos -= right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        }
        if (keyboard->isPressed(KeyCode::Space)) {
            m_cameraPos += Vector3({ 0, 1, 0 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::Shift)) {
            m_cameraPos -= Vector3({ 0, 1, 0 }) * m_cameraMoveSpeed;
        }
        m_cameraLookAt = m_cameraPos + forward;

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
        if (m_cursorVisible) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();
        }
        m_cursorVisible = !m_cursorVisible;
    }
}

Vector3 FpsController::getPosition() const { return m_cameraPos; }
Vector3 FpsController::getLookAt() const { return m_cameraLookAt; }
Quaternion FpsController::getForward() const { return Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 })); }
Vector3 FpsController::getForwardDir() const { return Quaternion::transform(getForward(), Vector3({ 0, 0, 1 })); }

void FpsController::setMoveSpeed(float moveSpeed) { m_cameraMoveSpeed = moveSpeed; }
float FpsController::getMoveSpeed() const { return m_cameraMoveSpeed; }

void FpsController::setRotateSpeed(float rotateSpeed) { m_cameraRotateSpeed = rotateSpeed; }
float FpsController::getRotateSpeed() const { return m_cameraRotateSpeed; }
}