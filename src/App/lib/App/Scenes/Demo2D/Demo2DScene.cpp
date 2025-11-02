#include <App/Scenes/Demo2D/Demo2DScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo2D {
// public
Demo2DScene::Demo2DScene()
    : m_spritePos({ -200, 0 })
    , m_spriteSize({ 100, 100 })
    , m_spriteColor({ 1.0f, 1.0f, 1.0f, 1.0f })
    , m_rectPos({ 0, 0 })
    , m_rectSize({ 100, 100 })
    , m_rectColor({ 1.0f, 1.0f, 1.0f, 1.0f })
    , m_circlePos({ 200, 0 })
    , m_circleSize({ 100, 100 })
    , m_circleColor({ 1.0f, 1.0f, 1.0f, 1.0f })
    , m_sampleTexture()
    , m_nextScene()
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
    if (!m_backButton) {
        m_backButton = std::make_unique<App::Common::Util::BackButton>();
        m_backButton->init(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"), [&]() -> void {
            m_nextScene = "Launcher";
        });
    }
    m_nextScene = "";
}

void Demo2DScene::onExit()
{
}

void Demo2DScene::onUpdate()
{
    m_backButton->update();
}

void Demo2DScene::onGui()
{
    ImGui::Begin("Demo2D");
    ImGui::SeparatorText("Sprite");
    ImGui::PushID(0);
    ImGui::InputFloat2("Pos", m_spritePos.data());
    ImGui::InputFloat2("Size", m_spriteSize.data());
    ImGui::ColorEdit4("Color", m_spriteColor.data());
    ImGui::PopID();
    ImGui::SeparatorText("Rect");
    ImGui::PushID(1);
    ImGui::InputFloat2("Pos", m_rectPos.data());
    ImGui::InputFloat2("Size", m_rectSize.data());
    ImGui::ColorEdit4("Color", m_rectColor.data());
    ImGui::PopID();
    ImGui::SeparatorText("Circle");
    ImGui::PushID(2);
    ImGui::InputFloat2("Pos", m_circlePos.data());
    ImGui::InputFloat2("Size", m_circleSize.data());
    ImGui::ColorEdit4("Color", m_circleColor.data());
    ImGui::PopID();
    ImGui::SeparatorText("Scene Transition");
    if (ImGui::Button("Exit")) {
        m_nextScene = "Launcher";
    }
    ImGui::End();
}

void Demo2DScene::onDraw3D()
{
}

void Demo2DScene::onDraw2D()
{
    m_backButton->draw(m_renderer);

    m_renderer->drawSprite(m_spritePos, m_spriteSize, 0.0f, m_sampleTexture, m_spriteColor);
    m_renderer->drawRect(m_rectPos, m_rectSize, 0.0f, m_rectColor);
    m_renderer->drawCircle(m_circlePos, m_circleSize, m_circleColor);

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