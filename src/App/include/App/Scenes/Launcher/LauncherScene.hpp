#pragma once
#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Util/FpsController.hpp>
#include <App/library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Launcher {
class LauncherScene : public IScene {
public:
    explicit LauncherScene();
    virtual ~LauncherScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    std::string m_nextScene;

    std::shared_ptr<RootPane> m_rootPane;
    std::shared_ptr<Renderer> m_renderer;
};
}