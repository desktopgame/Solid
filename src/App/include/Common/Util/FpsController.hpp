#pragma once
#include <library.hpp>

namespace App::Common::Util {
class FpsController {
public:
    explicit FpsController();

    void update();

    void lockCursor();
    void unlockCursor();

    void setPosition(const Vector3& position);
    Vector3 getPosition() const;

    void setAngleX(float angleX);
    float getAngleX() const;

    void setAngleY(float angleY);
    float getAngleY() const;

    Vector3 getLookAt() const;
    Quaternion getForward() const;
    Vector3 getForwardDir() const;

    void setMoveSpeed(float moveSpeed);
    float getMoveSpeed() const;
    float& getMoveSpeed();

    void setRotateSpeed(float rotateSpeed);
    float getRotateSpeed() const;
    float& getRotateSpeed();

private:
    Vector3 m_position;
    float m_angleX;
    float m_angleY;
    float m_moveSpeed;
    float m_rotateSpeed;
};
}