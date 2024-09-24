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
    , m_cameraMoveSpeed(0.04f)
    , m_cameraRotateSpeed(0.8f)
    , m_cursorVisible(true)
    , m_ioFile()
    , m_tileID()
    , m_tiles()
    , m_hintTileID()
    , m_hintTiles()
{
    std::string fileName = "assets/model1.csv";
    std::copy(fileName.begin(), fileName.end(), m_ioFile.begin());
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter(Renderer& renderer)
{
    m_tileID = renderer.rentTile(TileBufferKind::Medium);
    m_hintTileID = renderer.rentTile(TileBufferKind::Medium);

    m_tiles.push_back(Vector4({ 0, 0, 0, 0 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 1 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 2 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 3 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 4 }));
    m_tiles.push_back(Vector4({ 0, 0, 0, 5 }));
    renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tiles.data(), m_tiles.size());
}
void DebugScene::onExit(Renderer& renderer)
{
    renderer.releaseTile(TileBufferKind::Medium, m_tileID);
    renderer.releaseTile(TileBufferKind::Medium, m_hintTileID);
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
        m_cameraAngleX -= static_cast<float>(mouseDelta.y()) * m_cameraRotateSpeed;
        m_cameraAngleY += static_cast<float>(mouseDelta.x()) * m_cameraRotateSpeed;

        if (m_cameraAngleX <= -90.0f) {
            m_cameraAngleX = -89.0f;
        }
        if (m_cameraAngleX >= 90.0f) {
            m_cameraAngleX = 89.0f;
        }

        auto optHitPos = scanHintTiles(forward);
        renderer.batchTileArray(TileBufferKind::Medium, m_hintTileID, m_hintTiles.data(), m_hintTiles.size());

        if (mouse->isTrigger(Mouse::Button::Left)) {
            for (const auto& hintTile : m_hintTiles) {
                m_tiles.push_back(hintTile);
            }
            renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tiles.data(), m_tiles.size());
        } else if (mouse->isTrigger(Mouse::Button::Right)) {
            if (m_tiles.size() > 6 && optHitPos) {
                auto iter = std::remove_if(m_tiles.begin(), m_tiles.end(), [optHitPos](const auto& tile) -> bool {
                    return *optHitPos == (Vector3)tile;
                });
                m_tiles.erase(iter, m_tiles.end());
                renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tiles.data(), m_tiles.size());
            }
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
    ImGui::DragFloat("AngleX", &m_cameraAngleX);
    ImGui::DragFloat("AngleY", &m_cameraAngleY);
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

    ImGui::Begin("IO");
    ImGui::InputText("File", m_ioFile.data(), 32);
    if (ImGui::Button("Save")) {
        IO::serializeTile(std::string(m_ioFile.data()), m_tiles);
    }
    if (ImGui::Button("Load")) {
        IO::deserializeTile(std::string(m_ioFile.data()), m_tiles);
        renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tiles.data(), m_tiles.size());
    }
    ImGui::End();
};
void DebugScene::onDraw(Renderer& renderer)
{
    renderer.position(m_cameraPos);
    renderer.lookAt(m_cameraLookAt);

    renderer.drawTiles();

    renderer.drawCircle(Vector2({ 0, 0 }), Vector2({ 10, 10 }), Vector4({ 1, 1, 1, 1 }));
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

std::optional<Vector3> DebugScene::scanHintTiles(Vector3 forward)
{
    m_hintTiles.clear();
    auto hitTiles = Raycast::testTiles(m_cameraPos, forward, 10.0f);
    std::optional<Vector3> optHitPos = std::nullopt;
    for (const auto& hitTile : hitTiles) {
        for (const auto& otherTile : m_tiles) {
            if (hitTile.position == (Vector3)otherTile) {
                auto iter = std::find(TileBatch::s_normalVectorTable.begin(), TileBatch::s_normalVectorTable.end(), hitTile.normal);
                int32_t hitSide = std::distance(TileBatch::s_normalVectorTable.begin(), iter);
                int32_t tileSide = static_cast<int32_t>(otherTile.w()) % 10;
                if (hitSide == tileSide) {
                    auto placePos = hitTile.position + hitTile.normal;
                    bool found = std::find_if(m_tiles.begin(), m_tiles.end(), [placePos](const auto& e) -> bool {
                        return placePos == (Vector3)e;
                    }) != m_tiles.end();
                    if (!found) {
                        optHitPos = hitTile.position;
                        m_hintTiles.push_back(Vector4(placePos, 0 + getColorIndex()));
                        m_hintTiles.push_back(Vector4(placePos, 1 + getColorIndex()));
                        m_hintTiles.push_back(Vector4(placePos, 2 + getColorIndex()));
                        m_hintTiles.push_back(Vector4(placePos, 3 + getColorIndex()));
                        m_hintTiles.push_back(Vector4(placePos, 4 + getColorIndex()));
                        m_hintTiles.push_back(Vector4(placePos, 5 + getColorIndex()));
                        goto exit;
                    }
                }
            }
        }
    }
exit: {
}
    return optHitPos;
}
}