#pragma once
#include <library.hpp>

namespace App::Common {
class FpsController {
public:
    explicit FpsController();

    void update();

    Vector3 getPosition() const;
    Vector3 getLookAt() const;
    Quaternion getForward() const;
    Vector3 getForwardDir() const;

    void setMoveSpeed(float moveSpeed);
    float getMoveSpeed() const;

    void setRotateSpeed(float rotateSpeed);
    float getRotateSpeed() const;

private:
    Vector3 m_cameraPos;
    Vector3 m_cameraLookAt;
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;
    bool m_cursorVisible;
};
}