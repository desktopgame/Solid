#pragma once
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Game {
class GameScene : public IScene {
public:
    explicit GameScene();
    virtual ~GameScene();

    void onEnter(Renderer& renderer) override;
    void onExit(Renderer& renderer) override;

    void onUpdate(Renderer& renderer) override;
    void onGui(Renderer& renderer) override;
    void onDraw(Renderer& renderer) override;

    bool tryTransition(std::string& outNextScene) override;

private:
};
}