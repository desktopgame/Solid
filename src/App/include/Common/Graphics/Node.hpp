#include <library.hpp>
#include <memory>

namespace App::Common::Graphics {
class Node : public std::enable_shared_from_this<Node> {
public:
    explicit Node() = default;
    std::array<char, 16> name;
    Vector3 position;
    Vector3 size;
    Vector3 color;
    std::vector<std::shared_ptr<Node>> children;
    bool removed;

    static std::shared_ptr<Node> s_selected;
    static std::shared_ptr<Node> s_target;

    void update();
    void inspect();
    void gui(const std::shared_ptr<Node>& parent);
    void draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer);
};
}