#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// public
DebugScene::DebugScene()
    : m_tilePosition()
    , m_tileSide()
    , m_tilePallet()
    , m_tileColor()
    , m_cameraPos({ 0, 0, -1 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.01f)
    , m_cameraRotateSpeed(0.8f)
    , m_cursorVisible(true)
    , m_tileID()
    , m_tiles()
{
    m_tiles.push_back(Vector4({ 0, 0, 0, 0 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 1 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 2 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 3 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 4 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 5 }));
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter(Renderer& renderer)
{
    m_tileID = renderer.rentTile(TileBufferKind::Medium);
}
void DebugScene::onExit(Renderer& renderer)
{
    renderer.releaseTile(TileBufferKind::Medium, m_tileID);
};

void DebugScene::onUpdate(Renderer& renderer)
{
    (void)m_cameraAngleX;
    (void)m_cameraAngleY;

    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMosue();
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
        if (std::abs(mouseDelta.y()) > 0) {
            m_cameraAngleX -= static_cast<float>(mouseDelta.y()) * m_cameraRotateSpeed;
        }
        if (m_cameraAngleX <= -180.0f) {
            m_cameraAngleX = -179.0f;
        }
        if (m_cameraAngleX >= 180.0f) {
            m_cameraAngleX = 179.0f;
        }
        if (std::abs(mouseDelta.x()) > 0) {
            m_cameraAngleY += static_cast<float>(mouseDelta.x()) * m_cameraRotateSpeed;
        }
        if (m_cameraAngleY <= -180.0f) {
            m_cameraAngleY = -179.0f;
        }
        if (m_cameraAngleY >= 180.0f) {
            m_cameraAngleY = 179.0f;
        }
    }

    if (keyboard->isTrigger(KeyCode::E)) {
        if (m_cursorVisible) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();
        }
        m_cursorVisible = !m_cursorVisible;
    }
}

void DebugScene::onGui(Renderer& renderer)
{
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Pos", m_cameraPos.data());
    ImGui::DragFloat3("LookAt", m_cameraLookAt.data());
    ImGui::DragFloat("MoveSpeed", &m_cameraMoveSpeed, 0.01f);
    ImGui::DragFloat("RotationSpeed", &m_cameraRotateSpeed, 0.01f);
    ImGui::End();

    ImGui::Begin("Edit");
    ImGui::DragFloat3("Pos", m_tilePosition.data());
    ImGui::SliderInt("Side", &m_tileSide, 0, 5);
    ImGui::SliderInt("Pallet", &m_tilePallet, 0, 3);
    ImGui::SliderInt("Color", &m_tileColor, 0, 15);
    ImGui::End();

    ImGui::Begin("Cursor");
    auto pos = InputSystem::getInstance()->getMosue()->getPosition();
    auto delta = InputSystem::getInstance()->getMosue()->getDelta();
    ImGui::LabelText("Pos", "%d, %d", pos.x(), pos.y());
    ImGui::LabelText("Delta", "%d, %d", delta.x(), delta.y());
    ImGui::End();
};
void DebugScene::onDraw(Renderer& renderer)
{
    renderer.position(m_cameraPos);
    renderer.lookAt(m_cameraLookAt);

    renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tiles.data(), m_tiles.size());
    renderer.drawTiles();
};

bool DebugScene::tryTransition(std::string& outNextScene)
{
    return false;
};
// private
int32_t DebugScene::getColorIndex() const
{
    return ((m_tilePallet * 16) + m_tileColor) * 10;
}
}