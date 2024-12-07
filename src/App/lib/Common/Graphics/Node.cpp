#include <Common/Graphics/Node.hpp>
#include <fstream>
#include <imgui.h>

namespace App::Common::Graphics {
// public
std::shared_ptr<Node> Node::create()
{
    return std::shared_ptr<Node>(new Node());
}

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

void Node::draw(const std::shared_ptr<Renderer>& renderer)
{
    auto sp = m_parent.lock();
    draw(sp, renderer);
}

void Node::removeFromParent() { m_removed = true; }

std::shared_ptr<Node> Node::clone() const
{
    auto c = create();
    clone(shared_from_this(), c);
    return c;
}

void Node::serialize(const std::string& file, const std::shared_ptr<Node>& node)
{
    picojson::value::object root;
    serialize(root, node);

    picojson::value v = picojson::value(root);
    std::ofstream ofs(file);
    v.serialize(std::ostream_iterator<char>(ofs));
    ofs.close();
}
std::shared_ptr<Node> Node::deserialize(const std::string& file)
{
    std::ifstream ifs(file);
    std::istream_iterator<char> input(ifs);

    picojson::value v;
    auto rootNode = Node::create();
    std::string err;
    picojson::parse(v, input, std::istream_iterator<char>(), &err);
    deserialize(v.get<picojson::value::object>(), rootNode);
    ifs.close();
    return rootNode;
}

void Node::setName(const std::string& name) { std::copy(name.begin(), name.end(), m_name.begin()); }
void Node::setName(const std::array<char, 32>& name) { m_name = name; }
std::array<char, 32> Node::getName() const { return m_name; }
std::array<char, 32>& Node::getName() { return m_name; }

void Node::setLocalPosition(const Vector3& localPosition)
{
    m_isDirtyTransform = true;
    m_localPosition = localPosition;
}
Vector3 Node::getLocalPosition() const { return m_localPosition; }
Vector3& Node::getLocalPosition() { return m_localPosition; }
Vector3 Node::getGlobalPosition() const
{
    auto sp = m_parent.lock();
    if (sp) {
        return sp->getGlobalPosition() + m_localPosition;
    }
    return m_localPosition;
}

void Node::setLocalRotation(const Vector3& localRotation)
{
    m_isDirtyTransform = true;
    m_localRotation = localRotation;
}
Vector3 Node::getLocalRotation() const { return m_localRotation; }
Vector3& Node::getLocalRotation() { return m_localRotation; }
Matrix Node::getGlobalRotation() const
{
    Matrix m = Quaternion::toMatrix(Quaternion::angleAxis(m_localRotation.x(), Vector3({ 1, 0, 0 })) * Quaternion::angleAxis(m_localRotation.y(), Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(m_localRotation.z(), Vector3({ 0, 0, 1 })));
    auto sp = m_parent.lock();
    if (sp) {
        return m * sp->getGlobalRotation();
    }
    return m;
}

void Node::setSize(const Vector3& size)
{
    m_isDirtyTransform = true;
    m_size = size;
}
Vector3 Node::getSize() const { return m_size; }
Vector3& Node::getSize() { return m_size; }

void Node::setColor(const Vector3& color) { m_color = color; }
Vector3 Node::getColor() const { return m_color; }
Vector3& Node::getColor() { return m_color; }

Matrix Node::getLocalTransform()
{
    if (m_isDirtyTransform) {
        m_localTransform = Matrix::transform(
            Matrix::translate(m_localPosition),
            Quaternion::toMatrix(Quaternion::angleAxis(m_localRotation.x(), Vector3({ 1, 0, 0 })) * Quaternion::angleAxis(m_localRotation.y(), Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(m_localRotation.z(), Vector3({ 0, 0, 1 }))),
            Matrix());
        m_isDirtyTransform = false;
    }
    return m_localTransform;
}
Matrix Node::getGlobalTransform()
{
    auto sp = m_parent.lock();
    if (sp) {
        return getLocalTransform() * sp->getGlobalTransform();
    }
    return getLocalTransform();
}
std::array<Vector3, 8> Node::getEdges()
{
    Vector3 a = (m_size * Vector3({ -0.5f, 0.5f, 0.5f }));
    Vector3 b = (m_size * Vector3({ 0.5f, 0.5f, 0.5f }));
    Vector3 c = (m_size * Vector3({ -0.5f, 0.5f, -0.5f }));
    Vector3 d = (m_size * Vector3({ 0.5f, 0.5f, -0.5f }));
    Vector3 e = (m_size * Vector3({ -0.5f, -0.5f, 0.5f }));
    Vector3 f = (m_size * Vector3({ 0.5f, -0.5f, 0.5f }));
    Vector3 g = (m_size * Vector3({ -0.5f, -0.5f, -0.5f }));
    Vector3 h = (m_size * Vector3({ 0.5f, -0.5f, -0.5f }));

    Matrix m = getGlobalTransform();
    a = Matrix::multiply(m, a);
    b = Matrix::multiply(m, b);
    c = Matrix::multiply(m, c);
    d = Matrix::multiply(m, d);
    e = Matrix::multiply(m, e);
    f = Matrix::multiply(m, f);
    g = Matrix::multiply(m, g);
    h = Matrix::multiply(m, h);
    return std::array<Vector3, 8> { a, b, c, d, e, f, g, h };
}
Geom::OBB Node::getOBB() const
{
    Vector3 center = getGlobalPosition();
    Matrix m = getGlobalRotation();
    Vector3 xAxis = Vector3({ m[0][0], m[1][0], m[2][0] });
    Vector3 yAxis = Vector3({ m[0][1], m[1][1], m[2][1] });
    Vector3 zAxis = Vector3({ m[0][2], m[1][2], m[2][2] });

    Geom::OBB obb;
    obb.center = center;
    obb.axes = std::array<Vector3, 3> { xAxis, yAxis, zAxis };
    obb.size = m_size;
    return obb;
}

void Node::addChild(const std::shared_ptr<Node>& node)
{
    node->m_parent = shared_from_this();
    m_children.emplace_back(node);
}
std::shared_ptr<Node> Node::getChildAt(int32_t index) const { return m_children.at(index); }
int32_t Node::getChildrenCount() const { return static_cast<int32_t>(m_children.size()); }

bool Node::isRemoved() const { return m_removed; }
// private
void Node::clone(const std::shared_ptr<const Node>& src, const std::shared_ptr<Node>& dst)
{
    dst->m_localPosition = src->m_localPosition;
    dst->m_size = src->m_size;
    dst->m_color = src->m_color;

    dst->m_children.reserve(src->m_children.size());
    for (const auto& child : src->m_children) {
        auto newChild = create();
        clone(child, newChild);
        dst->addChild(newChild);
    }
}
void Node::serialize(picojson::value::object& parent, const std::shared_ptr<Node>& node)
{
    parent["name"] = picojson::value(std::string(node->m_name.data()));
    parent["posX"] = picojson::value(node->m_localPosition.x());
    parent["posY"] = picojson::value(node->m_localPosition.y());
    parent["posZ"] = picojson::value(node->m_localPosition.z());
    parent["sizeX"] = picojson::value(node->m_size.x());
    parent["sizeY"] = picojson::value(node->m_size.y());
    parent["sizeZ"] = picojson::value(node->m_size.z());
    parent["colorR"] = picojson::value(node->m_color.x());
    parent["colorG"] = picojson::value(node->m_color.y());
    parent["colorB"] = picojson::value(node->m_color.z());

    picojson::value::array children;
    for (int32_t i = 0; i < node->getChildrenCount(); i++) {
        auto c = node->getChildAt(i);
        picojson::value::object co;
        serialize(co, c);

        children.emplace_back(co);
    }
    parent["children"] = picojson::value(children);
}
void Node::deserialize(picojson::value::object& parent, const std::shared_ptr<Node>& node)
{
    std::string name = parent["name"].get<std::string>();
    node->setName(name);

    Vector3 pos;
    pos.x() = static_cast<float>(parent["posX"].get<double>());
    pos.y() = static_cast<float>(parent["posY"].get<double>());
    pos.z() = static_cast<float>(parent["posZ"].get<double>());
    node->setLocalPosition(pos);

    Vector3 size;
    size.x() = static_cast<float>(parent["sizeX"].get<double>());
    size.y() = static_cast<float>(parent["sizeY"].get<double>());
    size.z() = static_cast<float>(parent["sizeZ"].get<double>());
    node->setSize(size);

    Vector3 color;
    color.x() = static_cast<float>(parent["colorR"].get<double>());
    color.y() = static_cast<float>(parent["colorG"].get<double>());
    color.z() = static_cast<float>(parent["colorB"].get<double>());
    node->setColor(color);

    picojson::array children = parent["children"].get<picojson::value::array>();
    for (picojson::value& c : children) {
        auto cNode = Node::create();
        deserialize(c.get<picojson::value::object>(), cNode);

        node->addChild(cNode);
    }
}
Node::Node()
    : m_name()
    , m_localPosition()
    , m_localRotation()
    , m_size()
    , m_color()
    , m_localTransform()
    , m_isDirtyTransform(true)
    , m_parent()
    , m_children()
    , m_removed()
{
}
void Node::draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer)
{
    float thickness = 0.25f;
    renderer->pushMatrix(getLocalTransform());
    {
        Vector3 offset = Vector3({ thickness / 2.0f, thickness / 2.0f, 0 });
        Vector3 center = (m_size * Vector3({ 0.5f, 0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, thickness / 2.0f, 0 });
        Vector3 center = (m_size * Vector3({ -0.5f, 0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, -thickness / 2.0f, 0 });
        Vector3 center = (m_size * Vector3({ 0.5f, -0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, -thickness / 2.0f, 0 });
        Vector3 center = (m_size * Vector3({ -0.5f, -0.5f, 0 })) - offset;
        renderer->drawBox(center, Vector3({ thickness, thickness, m_size.z() }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, 0, thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ 0.5f, 0, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ thickness / 2.0f, 0, -thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ 0.5f, 0, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, 0, thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ -0.5f, 0, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ -thickness / 2.0f, 0, -thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ -0.5f, 0, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ thickness, m_size.y(), thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, thickness / 2.0f, thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ 0, 0.5f, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, -thickness / 2.0f, thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ 0, -0.5f, 0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, thickness / 2.0f, -thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ 0, 0.5f, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }
    {
        Vector3 offset = Vector3({ 0, -thickness / 2.0f, -thickness / 2.0f });
        Vector3 center = (m_size * Vector3({ 0, -0.5f, -0.5f })) - offset;
        renderer->drawBox(center, Vector3({ m_size.x(), thickness, thickness }), Quaternion(), Vector4(m_color, 1), false);
    }

    for (const auto& c : m_children) {
        c->draw(shared_from_this(), renderer);
    }
    renderer->popMatrix();
}
}