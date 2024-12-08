#pragma once
#include <library.hpp>
#include <memory>
#include <picojson/picojson.h>

namespace App::Common::Graphics {
class Node : public std::enable_shared_from_this<Node> {
public:
    static std::shared_ptr<Node> create();

    void update();
    void draw(const std::shared_ptr<Renderer>& renderer);
    void removeFromParent();

    void invalidate();
    void validate();

    std::shared_ptr<Node> clone() const;

    static void serialize(const std::string& file, const std::shared_ptr<Node>& node);
    static std::shared_ptr<Node> deserialize(const std::string& file);

    void setName(const std::string& name);
    void setName(const std::array<char, 32>& name);
    std::array<char, 32> getName() const;
    std::array<char, 32>& getName();

    void setLocalPosition(const Vector3& localPosition);
    Vector3 getLocalPosition() const;
    Vector3 getGlobalPosition();

    void setLocalRotation(const Vector3& localRotation);
    Vector3 getLocalRotation() const;
    Matrix getGlobalRotation() const;

    void setSize(const Vector3& size);
    Vector3 getSize() const;

    void setColor(const Vector3& color);
    Vector3 getColor() const;

    Matrix getLocalTransform();
    Matrix getGlobalTransform();
    std::array<Vector3, 8> getEdges();
    Geom::OBB getOBB();

    void addChild(const std::shared_ptr<Node>& node);
    std::shared_ptr<Node> getChildAt(int32_t index) const;
    int32_t getChildrenCount() const;

    bool isRemoved() const;

private:
    static void clone(const std::shared_ptr<const Node>& src, const std::shared_ptr<Node>& dst);
    static void serialize(picojson::value::object& parent, const std::shared_ptr<Node>& node);
    static void deserialize(picojson::value::object& parent, const std::shared_ptr<Node>& node);

    Node();
    void draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer);

    std::array<char, 32> m_name;
    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_size;
    Vector3 m_color;
    Matrix m_localTransform;
    bool m_isInvalid;
    std::weak_ptr<Node> m_parent;
    std::vector<std::shared_ptr<Node>> m_children;
    bool m_removed;
};
}