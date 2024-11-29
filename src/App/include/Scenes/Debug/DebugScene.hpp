#pragma once
#include <Common/Util/FpsController.hpp>
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Debug {
class DebugScene : public IScene {
public:
    explicit DebugScene();
    virtual ~DebugScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
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

    Common::Util::FpsController m_fpsController;
    std::shared_ptr<Node> m_rootNode;
    std::string m_nextScene;

    std::shared_ptr<Renderer> m_renderer;
};
}