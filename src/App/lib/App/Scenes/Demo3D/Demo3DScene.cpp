#include <App/Scenes/Demo3D/Demo3DScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo3D {
// public
Demo3DScene::Demo3DScene()
    : m_fpsController()
    , m_globalLightEnabled(true)
    , m_pointLightEnabled(true)
    , m_nextScene()
    , m_renderer()
{
}
Demo3DScene::~Demo3DScene() { }

void Demo3DScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_backButton) {
        m_backButton = std::make_unique<App::Common::Util::BackButton>();
        m_backButton->init(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"), [&]() -> void {
            m_nextScene = "Launcher";
        });
    }
    m_fpsController.lockCursor();
    m_fpsController.setPosition(Vector3({ 9, 5, -5 }));
    m_fpsController.setAngleX(0);
    m_fpsController.setAngleY(0);
    m_nextScene = "";
}

void Demo3DScene::onExit()
{
    m_fpsController.unlockCursor();
}

void Demo3DScene::onUpdate()
{
    m_fpsController.update();
    m_backButton->update();
}

void Demo3DScene::onGui()
{
    ImGui::Begin("Demo3D");
    ImGui::SeparatorText("GlobalLight");
    ImGui::PushID(0);
    ImGui::Checkbox("Enabled", &m_globalLightEnabled);
    ImGui::PopID();
    ImGui::SeparatorText("PointLight");
    ImGui::PushID(1);
    ImGui::Checkbox("Enabled", &m_pointLightEnabled);
    ImGui::PopID();
    ImGui::SeparatorText("Scene Transition");
    if (ImGui::Button("Exit")) {
        m_nextScene = "Launcher";
    }
    ImGui::End();
}
void Demo3DScene::onDraw3D()
{
    Camera::position(m_fpsController.getPosition());
    Camera::lookAt(m_fpsController.getLookAt());

    GlobalLight::toggle(m_globalLightEnabled);
    GlobalLight::set(Vector3::normalized(Vector3({ 1, 1, 0 })));

    PointLight::toggle(m_pointLightEnabled);
    PointLight::setCount(20 * 20);

    int32_t lightIndex = 0;
    for (int32_t i = 0; i < 20; i++) {
        for (int32_t j = 0; j < 20; j++) {
            float fx = i * 3;
            float fz = j * 3;
            float colorR = (lightIndex % 2) == 0 ? 1.0f : 0.0f;
            float colorG = (lightIndex % 2) == 0 ? 0.0f : 1.0f;
            float colorB = ::fmodf(static_cast<float>(lightIndex), 3.0f) / 3.0f;
            PointLight::set(lightIndex++, Vector3({ fx, 1.5f, fz }), 0.5f, 2.0f, Vector3({ colorR, colorG, colorB }));
        }
    }

    m_renderer->drawBox(Vector3({ 10, 0, 10 }), Vector3({ 20, 1, 20 }), Quaternion(), Color({ 1, 1, 1, 1 }), false);
}

void Demo3DScene::onDraw2D()
{
    m_backButton->draw(m_renderer);

    auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
    m_renderer->textFont(font);
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ 0, 330 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"ポイントライトのテストです。", Color({ 1, 1, 1, 1 }));
    m_renderer->drawText(Vector2({ 0, 300 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"WASDキーで移動できます。", Color({ 1, 1, 1, 1 }));
    m_renderer->drawText(Vector2({ 0, 270 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"Eキーでカーソルを表示できます。", Color({ 1, 1, 1, 1 }));
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