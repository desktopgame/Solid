#pragma once
#include <Common/FpsController.hpp>
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Demo {
class DemoScene : public IScene {
public:
    explicit DemoScene();
    virtual ~DemoScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    Common::FpsController m_fpsController;
    bool m_sceneCompleted;

    std::shared_ptr<Renderer> m_renderer;
};
}