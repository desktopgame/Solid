#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// public
DebugScene::DebugScene()
    : m_fpsController()
    , m_rootNode()
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
    if (!m_rootNode) {
        m_rootNode = std::make_shared<Common::Graphics::Node>();
        std::string name = "Root";
        std::copy(name.begin(), name.end(), m_rootNode->name.begin());
        m_rootNode->position = Vector3({ 0, 0, 0 });
        m_rootNode->size = Vector3({ 10, 10, 10 });
        m_rootNode->color = Vector3({ 1, 1, 1 });
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

    ImGui::Begin("Tree");
    m_rootNode->gui(nullptr);
    m_rootNode->update();
    ImGui::End();

    ImGui::Begin("Inspector");
    if (Common::Graphics::Node::s_target) {
        Common::Graphics::Node::s_target->inspect();
    }
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

    GlobalLight::enable();
    GlobalLight::set(Vector3({ 1, 1, 0 }));
    PointLight::enable();
    PointLight::setCount(0);
    m_renderer->drawBox(Vector3({ 0, 0, 0 }), Vector3({ 1, 1, 1 }), Quaternion(), Vector4({ 1, 1, 1, 1 }), false);

    m_rootNode->draw(nullptr, m_renderer);
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