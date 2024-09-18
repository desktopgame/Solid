#include <Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Debug {
// public
DebugScene::DebugScene()
    : m_tilePosition()
    , m_tileSide()
    , m_tileColor()
    , m_tileData()
    , m_cameraPos({ 0, 0, -1 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.01f)
    , m_cameraRotateSpeed(0.5f)
    , m_tileSubmit()
    , m_tileID(-1)
    , m_previewTileID(-1)
{
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter(Renderer& renderer)
{
    if (m_tileID == -1) {
        m_tileID = renderer.rentTile(TileBufferKind::Medium);
    }
    if (m_previewTileID == -1) {
        m_previewTileID = renderer.rentTile(TileBufferKind::UltraSmall);
    }
};
void DebugScene::onExit(Renderer& renderer)
{
    if (m_tileID != -1) {
        renderer.releaseTile(TileBufferKind::Medium, m_tileID);
    }
    if (m_previewTileID != -1) {
        renderer.releaseTile(TileBufferKind::UltraSmall, m_previewTileID);
    }
};

void DebugScene::onUpdate(Renderer& renderer)
{
    auto controller = InputSystem::getInstance()->getGamepad(0);
    if (!controller->isEnabled()) {
        return;
    }

    float leftStickX = static_cast<float>(controller->getLeftStickX()) / 32768.0f;
    float leftStickY = static_cast<float>(controller->getLeftStickY()) / 32768.0f;

    float rightStickX = static_cast<float>(controller->getRightStickX()) / 32768.0f;
    float rightStickY = static_cast<float>(controller->getRightStickY()) / 32768.0f;

    if (std::abs(rightStickY) > 0.5f) {
        m_cameraAngleX += rightStickY * m_cameraRotateSpeed;
    }
    if (std::abs(rightStickX) > 0.5f) {
        m_cameraAngleY += rightStickX * m_cameraRotateSpeed;
    }

    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    float lt = (static_cast<float>(controller->getLeftTrigger()) / 255.0f);
    float rt = (static_cast<float>(controller->getRightTrigger()) / 255.0f);
    if (std::abs(lt) > 0.0f) {
        m_cameraPos += forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
    } else if (std::abs(rt) > 0.0f) {
        m_cameraPos -= forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
    }
    if (leftStickX > 0.5f) {
        m_cameraPos += right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
    } else if (leftStickX < -0.5f) {
        m_cameraPos -= right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
    } else if (leftStickY > 0.5f) {
        m_cameraPos += Vector3({
            0,
            m_cameraMoveSpeed,
        });
    } else if (leftStickY < -0.5f) {
        m_cameraPos += Vector3({
            0,
            -m_cameraMoveSpeed,
        });
    }
    m_cameraLookAt = m_cameraPos + forward;
};
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
    ImGui::SliderInt("Color", &m_tileColor, 0, 63);
    ImGui::LabelText("TileCount", "%d", static_cast<int32_t>(m_tileData.size()));
    if (ImGui::Button("Submit")) {
        float w = static_cast<float>((m_tileColor * 10) + m_tileSide);
        m_tileData.push_back(Vector4(m_tilePosition, w));
        renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tileData.data(), m_tileData.size());
        m_tileSubmit = true;
    }
    ImGui::End();
};
void DebugScene::onDraw(Renderer& renderer)
{
    renderer.position(m_cameraPos);
    renderer.lookAt(m_cameraLookAt);

    float previewTileW = static_cast<float>((m_tileColor * 10) + m_tileSide);
    Vector4 perviewTile(m_tilePosition, previewTileW);
    renderer.batchTileArray(TileBufferKind::UltraSmall, m_previewTileID, &perviewTile, 1);
    renderer.drawTiles();
};

bool DebugScene::tryTransition(std::string& outNextScene)
{
    return false;
};
}