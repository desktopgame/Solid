#include <Common/Constants.hpp>
#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// Node
std::shared_ptr<DebugScene::Node> DebugScene::Node::s_selected = nullptr;
void DebugScene::Node::update()
{
    for (auto& c : children) {
        c->update();
    }
    auto iter = std::remove_if(children.begin(), children.end(), [](const auto& e) -> bool {
        return e->removed;
    });
    children.erase(iter, children.end());
}

void DebugScene::Node::gui(const std::shared_ptr<Node>& parent)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (shared_from_this() == s_selected) {
        flags = ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx(name.data(), flags)) {
        if (ImGui::IsItemClicked()) {
            s_selected = shared_from_this();
        }
        ImGui::InputText("Name", name.data(), 16);
        ImGui::DragFloat3("Pos", position.data(), 0.01f);
        ImGui::DragFloat3("Size", size.data(), 0.01f);
        ImGui::ColorEdit3("Color", color.data());
        if (ImGui::Button("New Node")) {
            auto child = std::make_shared<Node>();
            char buf[16];
            ::sprintf(buf, "Child[%d]", static_cast<int32_t>(children.size()));
            std::string childName = buf;
            std::copy(childName.begin(), childName.end(), child->name.begin());
            children.push_back(child);
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove This")) {
            removed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Select This")) {
            s_selected = shared_from_this();
        }

        for (const auto& c : children) {
            c->gui(shared_from_this());
        }
        ImGui::TreePop();
    }
}

void DebugScene::Node::draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer)
{
    renderer->drawBoxLine(position, size, Quaternion(), Vector4(color, 1.0f), 0.5f);

    for (const auto& c : children) {
        c->draw(shared_from_this(), renderer);
    }
}
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
        m_rootNode = std::make_shared<Node>();
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