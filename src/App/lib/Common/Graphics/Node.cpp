#include <Common/Graphics/Node.hpp>
#include <imgui.h>

namespace App::Common::Graphics {
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

void Node::removeFromParent() { m_removed = true; }

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

void Node::addChild(const std::shared_ptr<Node>& node) { m_children.emplace_back(node); }
std::shared_ptr<Node> Node::getChildAt(int32_t index) const { return m_children.at(index); }
int32_t Node::getChildrenCount() const { return static_cast<int32_t>(m_children.size()); }

bool Node::isRemoved() const { return m_removed; }
}