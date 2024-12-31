#pragma once
#include <Common/Util/FpsController.hpp>
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
    Common::Util::FpsController m_fpsController;
    bool m_isDraw2D;
    bool m_isDraw3D;
    Vector3 m_globalLightDir;
    Vector3 m_pointLightPos;
    std::vector<Vector3> m_pointLightPositions;
    bool m_sceneCompleted;

    std::shared_ptr<Buffer> m_particleVertexBuffer;
    std::shared_ptr<Buffer> m_particleIndexBuffer;
    std::shared_ptr<Buffer> m_particleInstanceSourceBuffer;
    std::shared_ptr<Buffer> m_particleInstanceBuffer;
    DualBuffer m_particleDualBuffer;
    int32_t m_particleIndexLength;

    std::shared_ptr<Renderer> m_renderer;
};
}