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
    Vector3 m_position;
    Vector3 m_lookAt;
    float m_angleX;
    float m_angleY;
    float m_moveSpeed;
    float m_rotateSpeed;
    bool m_cursorVisible;
};
}