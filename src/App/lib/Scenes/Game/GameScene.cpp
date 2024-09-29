#include <Common/Constants.hpp>
#include <Scenes/Game/GameScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
    : m_cameraPos({ 0, 3, -1 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_tileIDs()
    , m_tiles()
{
}
GameScene::~GameScene() { }

void GameScene::onEnter(Renderer& renderer)
{
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

    int32_t index = 0;
    while (index < m_tiles.size()) {
        int32_t length = Mathf::min(static_cast<int32_t>(m_tiles.size() - index), 4064 + 12);
        int32_t tileID = renderer.rentTile(TileBufferKind::UltraLarge);
        m_tileIDs.push_back(tileID);
        renderer.batchTileArray(TileBufferKind::UltraLarge, tileID, m_tiles.data() + index, length);

        index += length;
    }
}

void GameScene::onExit(Renderer& renderer)
{
    for (const auto& tileID : m_tileIDs) {
        renderer.releaseTile(TileBufferKind::UltraLarge, tileID);
    }
    m_tileIDs.clear();
    m_tiles.clear();
}

void GameScene::onUpdate(Renderer& renderer)
{
}

void GameScene::onGui(Renderer& renderer)
{
}

void GameScene::onDraw(Renderer& renderer)
{
    Camera::position(m_cameraPos);
    Camera::lookAt(m_cameraLookAt);

    renderer.drawTiles();
}

bool GameScene::tryTransition(std::string& outNextScene)
{
    return false;
}
// private
}