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
    Common::Util::FpsController m_fpsController;
    std::shared_ptr<Common::Graphics::Node> m_rootNode;
    std::string m_nextScene;

    std::shared_ptr<Renderer> m_renderer;
};
}