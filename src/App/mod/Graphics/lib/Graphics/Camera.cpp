#include <Graphics/Camera.hpp>
#include <Graphics/Screen.hpp>

namespace Lib::Graphics {
Lib::Math::Vector3 Camera::s_position = Lib::Math::Vector3({ 0, 0, -1 });
Lib::Math::Vector3 Camera::s_lookAt = Lib::Math::Vector3({ 0, 0, 0 });
float Camera::s_zNear = 1.0f;
float Camera::s_zFar = 1000.0f;
float Camera::s_fovY = 30.0f;
bool Camera::s_dirtyOrthoMatrix = true;
bool Camera::s_dirtyViewMatrix = true;
bool Camera::s_dirtyProjectionMatrix = true;
Lib::Math::Matrix Camera::s_orthoMatrix = Lib::Math::Matrix();
Lib::Math::Matrix Camera::s_viewMatrix = Lib::Math::Matrix();
Lib::Math::Matrix Camera::s_projectionMatrix = Lib::Math::Matrix();

void Camera::position(const Lib::Math::Vector3& position)
{
    s_position = position;
    s_dirtyViewMatrix = true;
}
void Camera::lookAt(const Lib::Math::Vector3& lookAt)
{
    s_lookAt = lookAt;
    s_dirtyViewMatrix = true;
}
void Camera::depthRange(float zNear, float zFar)
{
    s_zNear = zNear;
    s_zFar = zFar;
    s_dirtyProjectionMatrix = true;
}
void Camera::fovY(float fovY)
{
    s_fovY = fovY;
    s_dirtyProjectionMatrix = true;
}
Lib::Math::Matrix Camera::transform2D(const Lib::Math::Matrix& m)
{
    if (s_dirtyOrthoMatrix) {
        s_dirtyOrthoMatrix = false;
        s_orthoMatrix = Lib::Math::Matrix::ortho(
            static_cast<float>(Screen::getWidth()),
            static_cast<float>(Screen::getHeight()));
    }
    return m * s_orthoMatrix;
}
Lib::Math::Matrix Camera::transform3D(const Lib::Math::Matrix& m)
{
    if (s_dirtyViewMatrix) {
        s_dirtyViewMatrix = false;
        s_viewMatrix = Lib::Math::Matrix::lookAt(s_position, s_lookAt, Lib::Math::Vector3({ 0, 1, 0 }));
    }
    if (s_dirtyProjectionMatrix) {
        s_dirtyProjectionMatrix = false;
        s_projectionMatrix = Lib::Math::Matrix::perspective(s_fovY,
            static_cast<float>(Screen::getWidth()) / static_cast<float>(Screen::getHeight()),
            s_zNear,
            s_zFar);
    }
    return m * s_viewMatrix * s_projectionMatrix;
}
}