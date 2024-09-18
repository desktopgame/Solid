#include <Debug/DebugScene.hpp>
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

void DebugScene::onUpdate(Renderer& renderer) {
};
void DebugScene::onGui(Renderer& renderer)
{
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Pos", m_cameraPos.data());
    ImGui::DragFloat3("LookAt", m_cameraLookAt.data());
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