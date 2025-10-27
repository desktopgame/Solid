#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>

namespace Lib::Graphics {
/**
 * カメラ情報を静的に保持するクラスです。
 * Solidエンジンではカメラは一つしか存在しません。
 */
class Camera {
public:
    /**
     * カメラ位置を設定します。
     */
    static void position(const Math::Vector3& position);

    /**
     * カメラの注視点を設定します。
     * @param lookAt
     */
    static void lookAt(const Math::Vector3& lookAt);

    /**
     * クリッピング領域を設定します。
     * @param zNear
     * @param zFar
     */
    static void depthRange(float zNear, float zFar);

    /**
     * Fovを設定します。
     * @param fovY
     */
    static void fovY(float fovY);

    /**
     * 現在位置を返します。
     * @return
     */
    static Math::Vector3 getPosition();

    /**
     * 正投影行列を返します。
     * 主に2Dのレンダリングで使われます。
     * @return
     */
    static Math::Matrix getOrthoMatrix();

    /**
     * ビュー行列を返します。
     * @return
     */
    static Math::Matrix getLookAtMatrix();

    /**
     * プロジェクション行列を返します。
     * @return
     */
    static Math::Matrix getPerspectiveMatrix();

private:
    Camera() = delete;
    ~Camera() = delete;
    static Math::Vector3 s_position;
    static Math::Vector3 s_lookAt;
    static float s_zNear;
    static float s_zFar;
    static float s_fovY;

    static bool s_dirtyOrthoMatrix;
    static bool s_dirtyViewMatrix;
    static bool s_dirtyProjectionMatrix;
    static Math::Matrix s_orthoMatrix;
    static Math::Matrix s_viewMatrix;
    static Math::Matrix s_projectionMatrix;
};
}