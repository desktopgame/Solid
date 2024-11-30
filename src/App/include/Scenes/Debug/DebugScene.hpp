#pragma once
#include <Common/Graphics/Node.hpp>
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
    void guiEditNode(const std::shared_ptr<Common::Graphics::Node>& parent, const std::shared_ptr<Common::Graphics::Node>& node);
    void drawNode(const std::shared_ptr<Common::Graphics::Node>& parent, const std::shared_ptr<Common::Graphics::Node>& node, const std::shared_ptr<Renderer>& renderer);

    static std::shared_ptr<Common::Graphics::Node> s_selected;

    Common::Util::FpsController m_fpsController;
    std::shared_ptr<Common::Graphics::Node> m_rootNode;
    std::array<char, 32> m_ioFileName;
    std::string m_nextScene;

    std::shared_ptr<Renderer> m_renderer;
};
}