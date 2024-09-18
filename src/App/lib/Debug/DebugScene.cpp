#include <Debug/DebugScene.hpp>

namespace App::Debug {
// public
DebugScene::DebugScene() { }
DebugScene::~DebugScene() { }

void DebugScene::onEnter() {};
void DebugScene::onExit() {};

void DebugScene::onUpdate() {};
void DebugScene::onGui() {};
void DebugScene::onDraw(Renderer& renderer) {};

bool DebugScene::tryTransition(std::string& outNextScene)
{
    return false;
};
}