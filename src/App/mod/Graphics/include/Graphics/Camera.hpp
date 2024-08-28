#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>

namespace Lib::Graphics {
class Camera {
public:
    static void position(const Lib::Math::Vector3& position);
    static void lookAt(const Lib::Math::Vector3& lookAt);
    static void depthRange(float zNear, float zFar);
    static void fovY(float fovY);

    static Lib::Math::Matrix transform2D(const Lib::Math::Matrix& m);
    static Lib::Math::Matrix transform3D(const Lib::Math::Matrix& m);

private:
    static Lib::Math::Vector3 s_position;
    static Lib::Math::Vector3 s_lookAt;
    static float s_zNear;
    static float s_zFar;
    static float s_fovY;
    static bool s_dirtyOrthoMatrix;
    static bool s_dirtyViewMatrix;
    static bool s_dirtyProjectionMatrix;
    static Lib::Math::Matrix s_orthoMatrix;
    static Lib::Math::Matrix s_viewMatrix;
    static Lib::Math::Matrix s_projectionMatrix;
};
}