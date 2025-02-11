#pragma once
#include <App/Common/Graphics/ParticleSystem.hpp>
#include <App/Common/Graphics/SphericalParticle.hpp>
#include <App/Common/Util/FpsController.hpp>
#include <App/library.hpp>
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
    Common::Util::FpsController m_fpsController;
    bool m_isDraw2D;
    bool m_isDraw3D;
    Vector3 m_globalLightDir;
    Vector3 m_pointLightPos;
    std::vector<Vector3> m_pointLightPositions;
    std::shared_ptr<RootPane> m_root;
    bool m_sceneCompleted;
    std::shared_ptr<Renderer> m_renderer;
};
}