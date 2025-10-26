#include <App/Scenes/Demo2D/Demo2DScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo2D {
// public
Demo2DScene::Demo2DScene()
    : m_nextScene()
    , m_renderer()
{
}
Demo2DScene::~Demo2DScene() { }

void Demo2DScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
}

void Demo2DScene::onExit()
{
}

void Demo2DScene::onUpdate()
{
}

void Demo2DScene::onGui()
{
}

void Demo2DScene::onDraw3D()
{
}

void Demo2DScene::onDraw2D()
{
    m_renderer->drawRect(Vector2({ 0, 0 }), Vector2({ 100, 100 }), 0.0f, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
}

bool Demo2DScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
}