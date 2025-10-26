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
    if (!m_sampleTexture) {
        m_sampleTexture = Texture::create("./assets/Textures/aim.png");
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
    ImGui::Begin("Demo2D");
    ImGui::SeparatorText("Rect");
    ImGui::End();
}

void Demo2DScene::onDraw3D()
{
}

void Demo2DScene::onDraw2D()
{
    m_renderer->drawSprite(Vector2({ -200, 0 }), Vector2({ 100, 100 }), 0.0f, m_sampleTexture, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
    m_renderer->drawRect(Vector2({ 0, 0 }), Vector2({ 100, 100 }), 0.0f, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
    m_renderer->drawCircle(Vector2({ 200, 0 }), Vector2({ 100, 100 }), Color({ 1.0f, 1.0f, 1.0f, 1.0f }));

    auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
    m_renderer->textFont(font);
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ 0, 200 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"こんにちわ世界", Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
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