#include <Graphics/Camera.hpp>
#include <Graphics/Screen.hpp>

namespace Lib::Graphics {
Math::Vector3 Camera::s_position = Math::Vector3({ 0, 0, -1 });
Math::Vector3 Camera::s_lookAt = Math::Vector3({ 0, 0, 0 });
float Camera::s_zNear = 0.1f;
float Camera::s_zFar = 100.0f;
float Camera::s_fovY = 90.0f;

bool Camera::s_dirtyOrthoMatrix = true;
bool Camera::s_dirtyViewMatrix = true;
bool Camera::s_dirtyProjectionMatrix = true;
Math::Matrix Camera::s_orthoMatrix = Math::Matrix();
Math::Matrix Camera::s_viewMatrix = Math::Matrix();
Math::Matrix Camera::s_projectionMatrix = Math::Matrix();
// public
void Camera::position(const Math::Vector3& position)
{
    s_position = position;
    s_dirtyViewMatrix = true;
}

void Camera::lookAt(const Math::Vector3& lookAt)
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

Math::Vector3 Camera::getPosition() { return s_position; }

Math::Matrix Camera::getOrthoMatrix()
{
    if (s_dirtyOrthoMatrix) {
        s_dirtyOrthoMatrix = false;
        s_orthoMatrix = Math::Matrix::ortho(
            static_cast<float>(Screen::getWidth()),
            static_cast<float>(Screen::getHeight()));
    }
    return s_orthoMatrix;
}

Math::Matrix Camera::getLookAtMatrix()
{
    if (s_dirtyViewMatrix) {
        s_dirtyViewMatrix = false;
        s_viewMatrix = Math::Matrix::lookAt(s_position, s_lookAt, Math::Vector3({ 0, 1, 0 }));
    }
    return s_viewMatrix;
}

Math::Matrix Camera::getPerspectiveMatrix()
{
    if (s_dirtyProjectionMatrix) {
        s_dirtyProjectionMatrix = false;
        s_projectionMatrix = Math::Matrix::perspective(s_fovY,
            static_cast<float>(Screen::getWidth()) / static_cast<float>(Screen::getHeight()),
            s_zNear,
            s_zFar);
    }
    return s_projectionMatrix;
}
}