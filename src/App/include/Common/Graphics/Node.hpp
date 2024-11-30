#include <library.hpp>
#include <memory>

namespace App::Common::Graphics {
class Node : public std::enable_shared_from_this<Node> {
public:
    explicit Node() = default;

    static std::shared_ptr<Node> s_selected;
    static std::shared_ptr<Node> s_target;

    void update();
    void removeFromParent();

    void inspect();
    void gui(const std::shared_ptr<Node>& parent);
    void draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer);

    void setName(const std::string& name);
    void setName(const std::array<char, 16>& name);
    std::array<char, 16> getName() const;
    std::array<char, 16>& getName();

    void setPosition(const Vector3& position);
    Vector3 getPosition() const;
    Vector3& getPosition();

    void setSize(const Vector3& size);
    Vector3 getSize() const;
    Vector3& getSize();

    void setColor(const Vector3& color);
    Vector3 getColor() const;
    Vector3& getColor();

    bool isRemoved() const;

private:
    std::array<char, 16> m_name;
    Vector3 m_position;
    Vector3 m_size;
    Vector3 m_color;
    std::vector<std::shared_ptr<Node>> m_children;
    bool m_removed;
};
}