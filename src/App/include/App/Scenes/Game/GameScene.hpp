#pragma once
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <App/Scenes/Game/UI/Map.hpp>
#include <App/Scenes/Game/UI/Minimap.hpp>
#include <App/library.hpp>
#include <memory>
#include <optional>

#ifdef _DEBUG
#define GAMESCENE_PROFILE 1
#endif

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
    void onClickPauseClose();
    void onClickExitButton();

    Vector3 m_globalLightDir;

    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<FontMap> m_fontMap;
    std::shared_ptr<System::Field> m_field;
    std::shared_ptr<System::Entities::PlayerEntity> m_debugPlayer;
    std::shared_ptr<System::Entities::BasicEntity> m_debugEntity;
    std::shared_ptr<Texture> m_aimTexture;

    std::shared_ptr<RootPane> m_minimap;
    std::shared_ptr<UI::Map> m_map;
    std::shared_ptr<RootPane> m_pauseUI;
    bool m_requestPauseClose;

    std::string m_nextScene;

#if GAMESCENE_PROFILE
    const float k_fpsK = 0.05f;
    float m_avgTime = 0.0f;
#endif
};
}