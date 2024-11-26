#include <Common/Constants.hpp>
#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// public
DebugScene::DebugScene()
    : m_fpsController()
    , m_nextScene()
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
    m_nextScene = "";
    m_fpsController.setPosition(Vector3({ 0, 0, -10 }));
    m_fpsController.setAngleX(0);
    m_fpsController.setAngleY(0);
}

void DebugScene::onExit()
{
}

void DebugScene::onUpdate()
{
    m_fpsController.update();
}

void DebugScene::onGui()
{
    ImGui::Begin("Camera");
    ImGui::DragFloat("MoveSpeed", &m_fpsController.getMoveSpeed(), 0.01f);
    ImGui::DragFloat("RotateSpeed", &m_fpsController.getRotateSpeed(), 0.01f);
    ImGui::End();

    ImGui::Begin("Menu");
    ImGui::SeparatorText("Scene Transition");
    if (ImGui::Button("Demo")) {
        m_nextScene = "Demo";
    } else if (ImGui::Button("Game")) {
        m_nextScene = "Game";
    }
    ImGui::End();
}
void DebugScene::onDraw3D()
{
    Camera::position(m_fpsController.getPosition());
    Camera::lookAt(m_fpsController.getLookAt());

    m_renderer->drawBox(Vector3({ 0, 0, 0 }), Vector3({ 5, 5, 5 }), Quaternion(), Vector4({ 1, 1, 1, 1 }), false);
}

void DebugScene::onDraw2D()
{
}

bool DebugScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
}