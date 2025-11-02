#pragma once
#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Util/BackButton.hpp>
#include <App/Common/Util/FpsController.hpp>
#include <App/library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Demo3D {
class Demo3DScene : public IScene {
public:
    explicit Demo3DScene();
    virtual ~Demo3DScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    Common::Util::FpsController m_fpsController;
    bool m_globalLightEnabled;
    bool m_pointLightEnabled;
    std::string m_nextScene;
    std::shared_ptr<Renderer> m_renderer;

    std::unique_ptr<App::Common::Util::BackButton> m_backButton;
};
}