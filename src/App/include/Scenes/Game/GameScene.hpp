#pragma once
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Game {
class GameScene : public IScene {
public:
    explicit GameScene();
    virtual ~GameScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    static inline const float k_tileSize = 5.0f;

    Vector3 m_cameraPos;
    Vector3 m_cameraLookAt;
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;

    Vector3 m_globalLightDir;

    std::shared_ptr<Renderer> m_renderer;
};
}