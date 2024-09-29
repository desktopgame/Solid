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
    void onDraw() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    std::shared_ptr<Renderer> m_renderer;
    Vector3 m_cameraPos;
    Vector3 m_cameraLookAt;

    std::vector<int32_t> m_tileIDs;
    std::vector<Vector4> m_tiles;
};
}