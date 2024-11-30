#include <library.hpp>
#include <memory>

namespace App::Common::Graphics {
class Node : public std::enable_shared_from_this<Node> {
public:
    explicit Node() = default;

    void update();
    void draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer);
    void removeFromParent();

    void setName(const std::string& name);
    void setName(const std::array<char, 16>& name);
    std::array<char, 16> getName() const;
    std::array<char, 16>& getName();

    void setPosition(const Vector3& position);
    Vector3 getPosition() const;
    Vector3& getPosition();
    Vector3 getAbsolutePosition() const;

    void setSize(const Vector3& size);
    Vector3 getSize() const;
    Vector3& getSize();

    void setColor(const Vector3& color);
    Vector3 getColor() const;
    Vector3& getColor();

    void addChild(const std::shared_ptr<Node>& node);
    std::shared_ptr<Node> getChildAt(int32_t index) const;
    int32_t getChildrenCount() const;

    bool isRemoved() const;

private:
    std::array<char, 16> m_name;
    Vector3 m_position;
    Vector3 m_size;
    Vector3 m_color;
    std::weak_ptr<Node> m_parent;
    std::vector<std::shared_ptr<Node>> m_children;
    bool m_removed;
};
}