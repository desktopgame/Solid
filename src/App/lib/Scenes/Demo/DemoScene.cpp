#include <Common/Constants.hpp>
#include <Scenes/Demo/DemoScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo {
// public
DemoScene::DemoScene()
    : m_fpsController()
    , m_sceneCompleted()
    , m_renderer()
{
}
DemoScene::~DemoScene() { }

void DemoScene::onEnter()
{
    auto tex = Texture::create("./assets/tileNormal2.png");
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    m_sceneCompleted = false;
}
void DemoScene::onExit()
{
}

void DemoScene::onUpdate()
{
    m_fpsController.update();
}

void DemoScene::onGui()
{
    ImGui::Begin("Demo");
    if (ImGui::Button("Exit")) {
        m_sceneCompleted = true;
    }
    ImGui::End();
}
void DemoScene::onDraw3D()
{
    Camera::position(m_fpsController.getPosition());
    Camera::lookAt(m_fpsController.getLookAt());

    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(Vector3({ 1, 1, 0 })));

    PointLight::enable();
    PointLight::set(0, Vector3({ 8, 0, 6 }), 1, 5);
    PointLight::set(1, Vector3({ -8, 0, 6 }), 1, 5);
    PointLight::set(2, Vector3({ 20, 0, 6 }), 1, 5);
    PointLight::set(3, Vector3({ -20, 0, 6 }), 1, 5);
    PointLight::setCount(4);

    Quaternion q;
    m_renderer->drawPlane(Vector3({ 10, 0, 10 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawPlane(Vector3({ 10, 0, 20 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), true);
    m_renderer->drawBox(Vector3({ 25, 0, 10 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawBox(Vector3({ 25, 0, 20 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), true);
}

void DemoScene::onDraw2D()
{
    Vector2 backgroundCenter = Vector2({ -Screen::getWidth() / 4.0f, Screen::getHeight() / 4.0f });
    Vector2 backgroundSize = (Vector2)Screen::getSize() / 2.0f;
    m_renderer->drawRect(
        backgroundCenter,
        backgroundSize,
        0.0f,
        Color({ 1.0f, 1.0f, 1.0f, 1.0f }));

    m_renderer->drawCircle(backgroundCenter + Vector2({ -50, 0 }), Vector2({ 50, 50 }), Vector4({ 1, 0, 0, 1 }));
    m_renderer->drawRect(backgroundCenter + Vector2({ 50, 0 }), Vector2({ 50, 50 }), 45.0f, Vector4({ 0, 1, 0, 1 }));

    m_renderer->textFont(FontFactory::getInstance()->load("./assets/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(backgroundCenter - Vector2({ 0, 100 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"2D描画のテスト", Vector4({ 0, 0, 0, 1 }));
}

bool DemoScene::tryTransition(std::string& outNextScene)
{
    if (m_sceneCompleted) {
        outNextScene = "Debug";
    }
    return m_sceneCompleted;
}
// private
}