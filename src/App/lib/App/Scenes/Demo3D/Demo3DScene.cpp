#include <App/Scenes/Demo3D/Demo3DScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo3D {
// public
Demo3DScene::Demo3DScene()
    : m_nextScene()
    , m_renderer()
{
}
Demo3DScene::~Demo3DScene() { }

void Demo3DScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
}

void Demo3DScene::onExit()
{
}

void Demo3DScene::onUpdate()
{
}

void Demo3DScene::onGui()
{
}
void Demo3DScene::onDraw3D()
{
}

void Demo3DScene::onDraw2D()
{
}

bool Demo3DScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
}