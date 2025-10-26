#include <App/Scenes/Demo3D/Demo3DScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo3D {
// public
Demo3DScene::Demo3DScene()
    : m_fpsController()
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
    m_fpsController.lockCursor();
    m_fpsController.setPosition(Vector3({ 5, 3, -3 }));
    m_fpsController.setAngleX(0);
    m_fpsController.setAngleY(0);
}

void Demo3DScene::onExit()
{
    m_fpsController.unlockCursor();
}

void Demo3DScene::onUpdate()
{
    m_fpsController.update();
}

void Demo3DScene::onGui()
{
}
void Demo3DScene::onDraw3D()
{
    Camera::position(m_fpsController.getPosition());
    Camera::lookAt(m_fpsController.getLookAt());

    PointLight::enable();
    PointLight::setCount(4 * 4);

    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            float fx = i * 3;
            float fz = j * 3;
            PointLight::set(i * 4 + j, Vector3({ fx, 0, fz }), 1.0f, 3.0f);
        }
    }

    for (int32_t i = 0; i < 20; i++) {
        for (int32_t j = 0; j < 20; j++) {
            float colorR = static_cast<float>(i) / 10.0f;
            float colorG = static_cast<float>(j) / 10.0f;
            float colorB = static_cast<float>(i) / 10.0f;
            Vector4 color = Vector4({ colorR, colorG, colorB, 1 });
            m_renderer->drawPlane(Vector3({ static_cast<float>(i), 0, static_cast<float>(j) }), Vector2({ 1, 1 }), Quaternion::angleAxis(90.0f, Vector3({ -1, 0, 0 })), color, false);
        }
    }
}

void Demo3DScene::onDraw2D()
{
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