#include <Common/Graphics/Node.hpp>
#include <imgui.h>

namespace App::Common::Graphics {
std::shared_ptr<Node> Node::s_selected = nullptr;
std::shared_ptr<Node> Node::s_target = nullptr;
// public
void Node::update()
{
    for (auto& c : m_children) {
        c->update();
    }
    auto iter = std::remove_if(m_children.begin(), m_children.end(), [](const auto& e) -> bool {
        return e->isRemoved();
    });
    m_children.erase(iter, m_children.end());
}

void Node::removeFromParent() { m_removed = true; }

void Node::inspect()
{
    ImGui::SeparatorText("Layout");
    if (ImGui::Button("-PosX")) {
        if (s_selected) {
            Vector3 pos = s_selected->m_position - (s_selected->m_size * Vector3({ 0.5f, 0, 0 }));
            pos.x() -= m_size.x() * 0.5f;
            m_position = pos;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+PosX")) {
        if (s_selected) {
            Vector3 pos = s_selected->m_position + (s_selected->m_size * Vector3({ 0.5f, 0, 0 }));
            pos.x() += m_size.x() * 0.5f;
            m_position = pos;
        }
    }
    if (ImGui::Button("-PosY")) {
        if (s_selected) {
            Vector3 pos = s_selected->m_position - (s_selected->m_size * Vector3({ 0, 0.5f, 0 }));
            pos.y() -= m_size.y() * 0.5f;
            m_position = pos;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+PosY")) {
        if (s_selected) {
            Vector3 pos = s_selected->m_position + (s_selected->m_size * Vector3({ 0, 0.5f, 0 }));
            pos.y() += m_size.y() * 0.5f;
            m_position = pos;
        }
    }
    if (ImGui::Button("-PosZ")) {
        if (s_selected) {
            Vector3 pos = s_selected->m_position - (s_selected->m_size * Vector3({ 0, 0, 0.5f }));
            pos.z() -= m_size.z() * 0.5f;
            m_position = pos;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+PosZ")) {
        if (s_selected) {
            Vector3 pos = s_selected->m_position + (s_selected->m_size * Vector3({ 0, 0, 0.5f }));
            pos.z() += m_size.z() * 0.5f;
            m_position = pos;
        }
    }
}

void Node::gui(const std::shared_ptr<Node>& parent)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (shared_from_this() == s_selected) {
        flags = ImGuiTreeNodeFlags_Selected;
    }
    if (ImGui::TreeNodeEx(m_name.data(), flags)) {
        if (ImGui::IsItemClicked()) {
            s_selected = shared_from_this();
        }
        ImGui::InputText("Name", m_name.data(), 16);
        ImGui::DragFloat3("Pos", m_position.data(), 0.01f);
        ImGui::DragFloat3("Size", m_size.data(), 0.01f);
        ImGui::ColorEdit3("Color", m_color.data());
        if (ImGui::Button("New Node")) {
            auto child = std::make_shared<Node>();
            child->m_position = Vector3({ 0, 0, 0 });
            child->m_size = Vector3({ 10, 10, 10 });
            child->m_color = Vector3({ 1, 1, 1 });

            char buf[16];
            ::sprintf(buf, "Child[%d]", static_cast<int32_t>(m_children.size()));
            std::string childName = buf;
            std::copy(childName.begin(), childName.end(), child->m_name.begin());
            m_children.push_back(child);
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove This")) {
            m_removed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Inspect This")) {
            s_target = shared_from_this();
            s_selected = parent;
        }

        for (const auto& c : m_children) {
            c->gui(shared_from_this());
        }
        ImGui::TreePop();
    }
}

void Node::draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer)
{
    float thickness = 0.25f;
    {
        Vector3 offset = Vector3({ thickness / 2.0f, thickness / 2.0f, 0 });
        Vector3 center = m_position + (m_size * Vector3({ 0.5f, 0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, thickness / 2.0f, 0 });
        Vector3 center = m_position + (m_size * Vector3({ -0.5f, 0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, -thickness / 2.0f, 0 });
        Vector3 center = m_position + (m_size * Vector3({ 0.5f, -0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, -thickness / 2.0f, 0 });
        Vector3 center = m_position + (m_size * Vector3({ -0.5f, -0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, 0, thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ 0.5f, 0, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, 0, -thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ 0.5f, 0, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, 0, thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ -0.5f, 0, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, 0, -thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ -0.5f, 0, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, thickness / 2.0f, thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ 0, 0.5f, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, -thickness / 2.0f, thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ 0, -0.5f, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, thickness / 2.0f, -thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ 0, 0.5f, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, -thickness / 2.0f, -thickness / 2.0f });
        Vector3 center = m_position + (m_size * Vector3({ 0, -0.5f, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }

    for (const auto& c : m_children) {
        c->draw(shared_from_this(), renderer);
    }
}

void Node::setName(const std::string& name) { std::copy(name.begin(), name.end(), m_name.begin()); }
void Node::setName(const std::array<char, 16>& name) { m_name = name; }
std::array<char, 16> Node::getName() const { return m_name; }
std::array<char, 16>& Node::getName() { return m_name; }

void Node::setPosition(const Vector3& position) { m_position = position; }
Vector3 Node::getPosition() const { return m_position; }
Vector3& Node::getPosition() { return m_position; }

void Node::setSize(const Vector3& size) { m_size = size; }
Vector3 Node::getSize() const { return m_size; }
Vector3& Node::getSize() { return m_size; }

void Node::setColor(const Vector3& color) { m_color = color; }
Vector3 Node::getColor() const { return m_color; }
Vector3& Node::getColor() { return m_color; }

bool Node::isRemoved() const { return m_removed; }
}