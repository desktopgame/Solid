#pragma once
#include <App/library.hpp>

namespace App::Common::Util {
/**
 * 一人称視点の移動、視線制御を行うクラスです。
 * Minecraftライクな操作感を提供します。
 */
class FpsController {
public:
    explicit FpsController();

    /**
     * ユーザーの入力に基づいて移動、視線制御を行います。
     */
    void update();

    /**
     * カーソルを非表示化してウィンドウにロックします。
     */
    void lockCursor();

    /**
     * カーソルを表示してウィンドウからアンロックします。
     */
    void unlockCursor();

    /**
     * 位置を設定します。
     * @param position
     */
    void setPosition(const Vector3& position);

    /**
     * 位置を返します。
     * @return
     */
    Vector3 getPosition() const;

    /**
     * X軸方向の回転を設定します。
     */
    void setAngleX(float angleX);

    /**
     * X軸方向の回転を返します。
     * @return
     */
    float getAngleX() const;

    /**
     * Y軸方向の回転を設定します。
     */
    void setAngleY(float angleY);

    /**
     * Y軸方向の回転を返します。
     * @return
     */
    float getAngleY() const;

    /**
     * 位置に向きベクトルを加算した位置を返します。
     * @return
     */
    Vector3 getLookAt() const;

    /**
     * 向きをクォータニオンで返します。
     * @return
     */
    Quaternion getForward() const;

    /**
     * 向きをベクトルで返します。
     * @return
     */
    Vector3 getForwardDir() const;

    /**
     * 移動スピードを設定します。
     * @param moveSpeed
     */
    void setMoveSpeed(float moveSpeed);

    /**
     * 移動スピードを返します。
     * @return
     */
    float getMoveSpeed() const;

    /**
     * 移動スピードを返します。
     * @return
     */
    float& getMoveSpeed();

    /**
     * 回転スピードを設定します。
     * @param rotateSpeed
     */
    void setRotateSpeed(float rotateSpeed);

    /**
     * 回転スピードを返します。
     * @return
     */
    float getRotateSpeed() const;

    /**
     * 回転スピードを返します。
     * @return
     */
    float& getRotateSpeed();

private:
    Vector3 m_position;
    float m_angleX;
    float m_angleY;
    float m_moveSpeed;
    float m_rotateSpeed;
};
}