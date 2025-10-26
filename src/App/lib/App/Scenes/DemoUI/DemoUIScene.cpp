#include <App/Scenes/DemoUI/DemoUIScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::DemoUI {
// public
DemoUIScene::DemoUIScene()
    : m_nextScene()
    , m_renderer()
{
}
DemoUIScene::~DemoUIScene() { }

void DemoUIScene::onEnter()
{
}

void DemoUIScene::onExit()
{
}

void DemoUIScene::onUpdate()
{
}

void DemoUIScene::onGui()
{
}
void DemoUIScene::onDraw3D()
{
}

void DemoUIScene::onDraw2D()
{
}

bool DemoUIScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
}