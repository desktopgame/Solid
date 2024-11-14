#include <Common/Constants.hpp>
#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// public
DebugScene::DebugScene()
    : m_cameraPos({ 0, 0, -3 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.08f)
    , m_cameraRotateSpeed(0.8f)
    , m_cursorVisible(true)
    , m_editCompleted()
    , m_renderer()
{
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter()
{
    auto tex = Texture::create("./assets/tileNormal2.png");
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    m_editCompleted = false;
}
void DebugScene::onExit()
{
}

void DebugScene::onUpdate()
{
    (void)m_cameraAngleX;
    (void)m_cameraAngleY;

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

void DebugScene::onGui()
{
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Pos", m_cameraPos.data());
    ImGui::DragFloat3("LookAt", m_cameraLookAt.data());
    ImGui::DragFloat("AngleX", &m_cameraAngleX);
    ImGui::DragFloat("AngleY", &m_cameraAngleY);
    ImGui::DragFloat("MoveSpeed", &m_cameraMoveSpeed, 0.01f);
    ImGui::DragFloat("RotationSpeed", &m_cameraRotateSpeed, 0.01f);
    ImGui::End();

    ImGui::Begin("Cursor");
    auto pos = InputSystem::getInstance()->getMouse()->getPosition();
    auto delta = InputSystem::getInstance()->getMouse()->getDelta();
    ImGui::LabelText("Pos", "%d, %d", pos.x(), pos.y());
    ImGui::LabelText("Delta", "%d, %d", delta.x(), delta.y());
    ImGui::End();

    ImGui::Begin("Menu");
    if (ImGui::Button("Exit")) {
        m_editCompleted = true;
    }
    ImGui::End();
}
void DebugScene::onDraw3D()
{
    Camera::position(m_cameraPos);
    Camera::lookAt(m_cameraLookAt);

    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(Vector3({ 1, 1, 0 })));

    PointLight::enable();
    PointLight::set(0, Vector3({ 8, 0, 6 }), 1, 5);
    PointLight::set(1, Vector3({ -8, 0, 6 }), 1, 5);
    PointLight::set(2, Vector3({ 20, 0, 6 }), 1, 5);
    PointLight::set(3, Vector3({ -20, 0, 6 }), 1, 5);
    PointLight::setCount(4);

    Quaternion q;
    m_renderer->drawPlane(Vector3({ 10, 0, 10 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawPlane(Vector3({ 10, 0, 20 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), true);
    m_renderer->drawBox(Vector3({ 25, 0, 10 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawBox(Vector3({ 25, 0, 20 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), true);
}

void DebugScene::onDraw2D()
{
    m_renderer->drawCircle(Vector2({ 0, 0 }), Vector2({ 50, 50 }), Vector4({ 1, 1, 1, 1 }));
    m_renderer->drawRect(Vector2({ 100, 100 }), Vector2({ 50, 50 }), 45.0f, Vector4({ 1, 0, 0, 1 }));
    m_renderer->textFont(FontFactory::getInstance()->load("./assets/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ 200, 200 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"こんにちわ", Vector4({ 1, 1, 1, 1 }));
}

bool DebugScene::tryTransition(std::string& outNextScene)
{
    if (m_editCompleted) {
        outNextScene = "Game";
    }
    return m_editCompleted;
}
// private
}