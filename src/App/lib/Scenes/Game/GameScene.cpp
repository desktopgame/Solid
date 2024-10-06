#include <Common/Constants.hpp>
#include <Scenes/Game/GameScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
    : m_renderer()
    , m_tileRenderer()
    , m_cameraPos({ 0, 3, -1 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.04f)
    , m_cameraRotateSpeed(0.8f)
    , m_tileTicket()
    , m_tiles()
{
}
GameScene::~GameScene() { }

void GameScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_tileRenderer) {
        m_tileRenderer = std::make_shared<TileRenderer>(Common::Constants::k_tileSize);
    }

    int32_t color = 0;
    int32_t size = 100;
    for (int32_t i = -size; i < size; i++) {
        for (int32_t j = -size; j < size; j++) {
            float x = static_cast<float>(j) * Common::Constants::k_tileSize;
            float z = static_cast<float>(i) * Common::Constants::k_tileSize;
            Vector4 v = Vector4({ x, 0.0f, z, static_cast<float>(color + 0) });
            m_tiles.push_back(v);

            color += 10;
            if (color >= 640) {
                color = 0;
            }
        }
    }

    m_tileTicket = m_tileRenderer->rentTileTicket(m_tiles.size());
    m_tileTicket->batchTileArray(m_tiles.data());

    if (Cursor::isVisible()) {
        Cursor::hide();
        Cursor::lock(Engine::getInstance()->getWindow());
    }
}

void GameScene::onExit()
{
    m_tileRenderer->releaseTileTicket(m_tileTicket);
    m_tiles.clear();
}

void GameScene::onUpdate()
{
    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();

    if (Cursor::isLocked()) {
        if (keyboard->isPressed(KeyCode::W)) {
            m_cameraPos += forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::S)) {
            m_cameraPos -= forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::D)) {
            m_cameraPos += right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::A)) {
            m_cameraPos -= right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        }
        if (keyboard->isPressed(KeyCode::Space)) {
            m_cameraPos += Vector3({ 0, 1, 0 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::Shift)) {
            m_cameraPos -= Vector3({ 0, 1, 0 }) * m_cameraMoveSpeed;
        }
        m_cameraLookAt = m_cameraPos + forward;

        auto mouseDelta = mouse->getDelta();
        m_cameraAngleX -= static_cast<float>(mouseDelta.y()) * m_cameraRotateSpeed;
        m_cameraAngleY += static_cast<float>(mouseDelta.x()) * m_cameraRotateSpeed;

        if (m_cameraAngleX <= -90.0f) {
            m_cameraAngleX = -89.0f;
        }
        if (m_cameraAngleX >= 90.0f) {
            m_cameraAngleX = 89.0f;
        }
    }

    if (keyboard->isTrigger(KeyCode::E)) {
        if (Cursor::isVisible()) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();
        }
    }
}

void GameScene::onGui()
{
}

void GameScene::onDraw()
{
    Camera::position(m_cameraPos);
    Camera::lookAt(m_cameraLookAt);

    m_tileRenderer->drawTiles();
}

bool GameScene::tryTransition(std::string& outNextScene)
{
    return false;
}
// private
}