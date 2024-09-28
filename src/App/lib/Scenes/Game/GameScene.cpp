#include <Common/Constants.hpp>
#include <Scenes/Game/GameScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
{
}
GameScene::~GameScene() { }

void GameScene::onEnter(Renderer& renderer)
{
}

void GameScene::onExit(Renderer& renderer) {
};

void GameScene::onUpdate(Renderer& renderer)
{
}

void GameScene::onGui(Renderer& renderer) {
};

void GameScene::onDraw(Renderer& renderer) {
};

bool GameScene::tryTransition(std::string& outNextScene)
{
    return false;
};
// private
}