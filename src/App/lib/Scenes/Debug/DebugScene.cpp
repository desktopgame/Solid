#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
std::shared_ptr<Common::Graphics::Node> DebugScene::s_selected = nullptr;
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

    m_rootNode = Common::Graphics::Node::create();
    m_rootNode->setName("Root");
    m_rootNode->setLocalPosition(Vector3({ 0, 0, 0 }));
    m_rootNode->setSize(Vector3({ 10, 10, 10 }));
    m_rootNode->setColor(Vector3({ 1, 1, 1 }));

    std::string ioFileName = "./rawdata/Models/sample.json";
    std::fill(m_ioFileName.begin(), m_ioFileName.end(), '\0');
    std::copy(ioFileName.begin(), ioFileName.end(), m_ioFileName.begin());

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
    guiEditNode(nullptr, m_rootNode);
    m_rootNode->update();
    ImGui::End();

    ImGui::Begin("IO");
    ImGui::InputText("File", m_ioFileName.data(), 32);
    if (ImGui::Button("Serialize")) {
        Common::Graphics::Node::serialize(std::string(m_ioFileName.data()), m_rootNode);
    }
    if (ImGui::Button("Deserialize")) {
        m_rootNode = Common::Graphics::Node::deserialize(std::string(m_ioFileName.data()));
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

    m_rootNode->draw(m_renderer);
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
void DebugScene::guiEditNode(const std::shared_ptr<Common::Graphics::Node>& parent, const std::shared_ptr<Common::Graphics::Node>& node)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (node == s_selected) {
        flags = ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx(node->getName().data(), flags)) {
        if (ImGui::IsItemClicked()) {
            s_selected = node;
        }
        ImGui::InputText("Name", node->getName().data(), 16);

        Vector3 localPos = node->getLocalPosition();
        if (ImGui::DragFloat3("Pos", localPos.data(), 0.01f)) {
            node->setLocalPosition(localPos);
        }

        Vector3 localRot = node->getLocalRotation();
        if (ImGui::DragFloat3("Rot", localRot.data(), 1.0f, 0.0f, 360.0f)) {
            node->setLocalRotation(localRot);
        }

        Vector3 size = node->getSize();
        if (ImGui::DragFloat3("Size", size.data(), 0.01f)) {
            node->setSize(size);
        }

        Vector3 color = node->getColor();
        if (ImGui::ColorEdit3("Color", color.data())) {
            node->setColor(color);
        }
        if (ImGui::Button("New Node")) {
            auto child = Common::Graphics::Node::create();
            child->setLocalPosition(Vector3({ 0, 0, 0 }));
            child->setSize(Vector3({ 10, 10, 10 }));
            child->setColor(Vector3({ 1, 1, 1 }));

            char buf[16];
            ::sprintf(buf, "Child[%d]", node->getChildrenCount());
            std::string childName = buf;
            child->setName(childName);
            node->addChild(child);
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove This")) {
            node->removeFromParent();
        }

        if (ImGui::Button("-X")) {
            Vector3 pos = -(parent->getSize() * Vector3({ 0.5f, 0, 0 }));
            pos.x() -= node->getSize().x() * 0.5f;
            node->setLocalPosition(pos);
        }
        ImGui::SameLine();
        if (ImGui::Button("+X")) {
            Vector3 pos = (parent->getSize() * Vector3({ 0.5f, 0, 0 }));
            pos.x() += node->getSize().x() * 0.5f;
            node->setLocalPosition(pos);
        }
        ImGui::SameLine();
        if (ImGui::Button("-Y")) {
            Vector3 pos = -(parent->getSize() * Vector3({ 0, 0.5f, 0 }));
            pos.y() -= node->getSize().y() * 0.5f;
            node->setLocalPosition(pos);
        }
        ImGui::SameLine();
        if (ImGui::Button("+Y")) {
            Vector3 pos = (parent->getSize() * Vector3({ 0, 0.5f, 0 }));
            pos.y() += node->getSize().y() * 0.5f;
            node->setLocalPosition(pos);
        }
        ImGui::SameLine();
        if (ImGui::Button("-Z")) {
            Vector3 pos = -(parent->getSize() * Vector3({ 0, 0, 0.5f }));
            pos.z() -= node->getSize().z() * 0.5f;
            node->setLocalPosition(pos);
        }
        ImGui::SameLine();
        if (ImGui::Button("+Z")) {
            Vector3 pos = (parent->getSize() * Vector3({ 0, 0, 0.5f }));
            pos.z() += node->getSize().z() * 0.5f;
            node->setLocalPosition(pos);
        }

        for (int32_t i = 0; i < node->getChildrenCount(); i++) {
            guiEditNode(node, node->getChildAt(i));
        }
        ImGui::TreePop();
    }
}
void DebugScene::drawNode(const std::shared_ptr<Common::Graphics::Node>& parent, const std::shared_ptr<Common::Graphics::Node>& node, const std::shared_ptr<Renderer>& renderer)
{
    node->draw(renderer);
}
}