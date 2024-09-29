#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>

namespace Lib::Graphics {
class Camera {
public:
    static void position(const Math::Vector3& position);
    static void lookAt(const Math::Vector3& lookAt);
    static void depthRange(float zNear, float zFar);
    static void fovY(float fovY);

    static Math::Matrix getOrthoMatrix();
    static Math::Matrix getLookAtMatrix();
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