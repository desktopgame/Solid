#pragma once
#include <Scenes/Game/System/Entities/BasicEntity.hpp>
#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Field.hpp>
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
    Vector3 m_globalLightDir;

    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<System::Field> m_field;
    std::shared_ptr<System::Entities::PlayerEntity> m_debugPlayer;
    std::shared_ptr<System::Entities::BasicEntity> m_debugEntity;
    std::shared_ptr<Texture> m_aimTexture;

#if _DEBUG
    const float k_fpsK = 0.05f;
    float m_avgTime = 0.0f;
#endif
};
}