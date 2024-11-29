#include <Common/Graphics/Node.hpp>
#include <imgui.h>

namespace App::Common::Graphics {
// Node
std::shared_ptr<Node> Node::s_selected = nullptr;
std::shared_ptr<Node> Node::s_target = nullptr;
void Node::update()
{
    for (auto& c : children) {
        c->update();
    }
    auto iter = std::remove_if(children.begin(), children.end(), [](const auto& e) -> bool {
        return e->removed;
    });
    children.erase(iter, children.end());
}

void Node::inspect()
{
    ImGui::SeparatorText("Layout");
    if (ImGui::Button("-PosX")) {
        if (s_selected) {
            Vector3 pos = s_selected->position - (s_selected->size * Vector3({ 0.5f, 0, 0 }));
            pos.x() -= size.x() * 0.5f;
            position = pos;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+PosX")) {
        if (s_selected) {
            Vector3 pos = s_selected->position + (s_selected->size * Vector3({ 0.5f, 0, 0 }));
            pos.x() += size.x() * 0.5f;
            position = pos;
        }
    }
    if (ImGui::Button("-PosY")) {
        if (s_selected) {
            Vector3 pos = s_selected->position - (s_selected->size * Vector3({ 0, 0.5f, 0 }));
            pos.y() -= size.y() * 0.5f;
            position = pos;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+PosY")) {
        if (s_selected) {
            Vector3 pos = s_selected->position + (s_selected->size * Vector3({ 0, 0.5f, 0 }));
            pos.y() += size.y() * 0.5f;
            position = pos;
        }
    }
    if (ImGui::Button("-PosZ")) {
        if (s_selected) {
            Vector3 pos = s_selected->position - (s_selected->size * Vector3({ 0, 0, 0.5f }));
            pos.z() -= size.z() * 0.5f;
            position = pos;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+PosZ")) {
        if (s_selected) {
            Vector3 pos = s_selected->position + (s_selected->size * Vector3({ 0, 0, 0.5f }));
            pos.z() += size.z() * 0.5f;
            position = pos;
        }
    }
}

void Node::gui(const std::shared_ptr<Node>& parent)
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
            child->position = Vector3({ 0, 0, 0 });
            child->size = Vector3({ 10, 10, 10 });
            child->color = Vector3({ 1, 1, 1 });

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
        if (ImGui::Button("Inspect This")) {
            s_target = shared_from_this();
            s_selected = parent;
        }

        for (const auto& c : children) {
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
        Vector3 center = position + (size * Vector3({ 0.5f, 0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, size.z() }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, thickness / 2.0f, 0 });
        Vector3 center = position + (size * Vector3({ -0.5f, 0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, size.z() }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, -thickness / 2.0f, 0 });
        Vector3 center = position + (size * Vector3({ 0.5f, -0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, size.z() }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, -thickness / 2.0f, 0 });
        Vector3 center = position + (size * Vector3({ -0.5f, -0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, size.z() }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, 0, thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ 0.5f, 0, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, size.y(), thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, 0, -thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ 0.5f, 0, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, size.y(), thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, 0, thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ -0.5f, 0, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, size.y(), thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, 0, -thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ -0.5f, 0, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, size.y(), thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, thickness / 2.0f, thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ 0, 0.5f, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ size.x(), thickness, thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, -thickness / 2.0f, thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ 0, -0.5f, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ size.x(), thickness, thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, thickness / 2.0f, -thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ 0, 0.5f, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ size.x(), thickness, thickness }), Quaternion(), Vector4(color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, -thickness / 2.0f, -thickness / 2.0f });
        Vector3 center = position + (size * Vector3({ 0, -0.5f, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ size.x(), thickness, thickness }), Quaternion(), Vector4(color, 1), false);
    }

    for (const auto& c : children) {
        c->draw(shared_from_this(), renderer);
    }
}
}