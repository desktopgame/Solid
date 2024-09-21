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
    , m_cameraRotateSpeed(0.5f)
{
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter(Renderer& renderer) { }
void DebugScene::onExit(Renderer& renderer) {};

void DebugScene::onUpdate(Renderer& renderer)
{
    (void)m_cameraAngleX;
    (void)m_cameraAngleY;
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
};
void DebugScene::onDraw(Renderer& renderer)
{
    renderer.position(m_cameraPos);
    renderer.lookAt(m_cameraLookAt);

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