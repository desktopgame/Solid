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
    , m_tileData()
    , m_backTileData()
    , m_mode(Mode::Camera)
    , m_cameraPos({ 0, 0, -1 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.01f)
    , m_cameraRotateSpeed(0.5f)
    , m_ioFile()
    , m_tileSubmit()
    , m_tileID(-1)
    , m_backTileID(-1)
    , m_previewTileID(-1)
{
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter(Renderer& renderer)
{
    if (m_tileID == -1) {
        m_tileID = renderer.rentTile(TileBufferKind::Medium);
    }
    if (m_backTileID == -1) {
        m_backTileID = renderer.rentTile(TileBufferKind::Medium);
    }
    if (m_previewTileID == -1) {
        m_previewTileID = renderer.rentTile(TileBufferKind::UltraSmall);
    }

    std::string defaultFile("assets/model1.csv");
    std::copy(defaultFile.begin(), defaultFile.end(), m_ioFile.begin());
};
void DebugScene::onExit(Renderer& renderer)
{
    if (m_tileID != -1) {
        renderer.releaseTile(TileBufferKind::Medium, m_tileID);
    }
    if (m_backTileID != -1) {
        renderer.releaseTile(TileBufferKind::Medium, m_backTileID);
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

    float lt = (static_cast<float>(controller->getLeftTrigger()) / 255.0f);
    float rt = (static_cast<float>(controller->getRightTrigger()) / 255.0f);

    bool buttonA = controller->isTrigger(Gamepad::Button::A);
    bool buttonB = controller->isTrigger(Gamepad::Button::B);
    bool buttonY = controller->isPressed(Gamepad::Button::Y);
    bool sholderL = controller->isTrigger(Gamepad::Button::LeftSholder);
    bool sholderR = controller->isTrigger(Gamepad::Button::RightSholder);

    if (m_mode == Mode::Camera) {
        if (std::abs(rightStickY) > 0.5f) {
            m_cameraAngleX += rightStickY * m_cameraRotateSpeed;
        }
        if (std::abs(rightStickX) > 0.5f) {
            m_cameraAngleY += rightStickX * m_cameraRotateSpeed;
        }

        auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
        auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
        auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

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
    } else if (m_mode == Mode::Edit) {
        if (buttonB) {
            m_tileSide++;
            if (m_tileSide >= 6) {
                m_tileSide = 0;
            }
        } else if (buttonA) {
            putTile(renderer);
        }
        if (sholderL) {
            m_tilePosition -= TileBatch::s_normalVectorTable.at(m_tileSide);
        }
        if (sholderR) {
            m_tilePosition += TileBatch::s_normalVectorTable.at(m_tileSide);
        }
    }
    if (buttonY && sholderR) {
        m_mode = m_mode == Mode::Camera ? Mode::Edit : Mode::Camera;
    }
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
    ImGui::SliderInt("Pallet", &m_tilePallet, 0, 3);
    ImGui::SliderInt("Color", &m_tileColor, 0, 15);
    ImGui::LabelText("TileCount", "%d", static_cast<int32_t>(m_tileData.size()));
    ImGui::LabelText("Mode", m_mode == Mode::Camera ? "Camera" : "Edit");
    if (ImGui::Button("Update")) {
        updateTile(renderer);
    }
    if (ImGui::Button("Remove")) {
        removeTile(renderer);
    }
    if (ImGui::Button("Submit")) {
        putTile(renderer);
    }
    ImGui::End();

    ImGui::Begin("IO");
    ImGui::InputText("File", m_ioFile.data(), 32);
    if (ImGui::Button("Save")) {
        IO::serializeTile(std::string(m_ioFile.data()), m_tileData);
    }
    if (ImGui::Button("Load")) {
        IO::deserializeTile(std::string(m_ioFile.data()), m_tileData);
        renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tileData.data(), m_tileData.size());

        m_backTileData.clear();
        for (const auto& tile : m_tileData) {
            Vector3 pos = (Vector3)tile;
            int32_t side = static_cast<int32_t>(tile.w()) % 10;
            int32_t color = static_cast<int32_t>(tile.w()) / 10;

            Vector3 backPos = pos + TileBatch::s_normalVectorTable.at(side);
            int32_t backSide = TileBatch::s_tileReverseTable.at(side);
            m_backTileData.push_back(Vector4(backPos, (color * 10) + backSide));
        }
        renderer.batchTileArray(TileBufferKind::Medium, m_backTileID, m_backTileData.data(), m_backTileData.size());
    }
    ImGui::End();

    ImGui::Begin("Input");
    for (int32_t i = 0; i < (int32_t)KeyCode::Count; i++) {
        std::string label = "";
        switch (InputSystem::getInstance()->getKeyboard()->getState((KeyCode)i)) {
        case ButtonState::None:
            label = "";
            break;
        case ButtonState::Trigger:
            label = "Trigger";
            break;
        case ButtonState::Pressed:
            label = "Pressed";
            break;
        case ButtonState::Released:
            label = "Released";
            break;
        }
        switch ((KeyCode)i) {
        case KeyCode::W:
            ImGui::LabelText("W", "%s", label.c_str());
            break;
        case KeyCode::A:
            ImGui::LabelText("A", "%s", label.c_str());
            break;
        case KeyCode::S:
            ImGui::LabelText("S", "%s", label.c_str());
            break;
        case KeyCode::D:
            ImGui::LabelText("D", "%s", label.c_str());
            break;
        case KeyCode::Space:
            ImGui::LabelText("Space", "%s", label.c_str());
            break;
        case KeyCode::ArrowUp:
            ImGui::LabelText("Up", "%s", label.c_str());
            break;
        case KeyCode::ArrowDown:
            ImGui::LabelText("Down", "%s", label.c_str());
            break;
        case KeyCode::ArrowLeft:
            ImGui::LabelText("Left", "%s", label.c_str());
            break;
        case KeyCode::ArrowRight:
            ImGui::LabelText("Right", "%s", label.c_str());
            break;
        default:
            break;
        }
    }

    for (int32_t i = 0; i < (int32_t)Mouse::Button::Count; i++) {
        std::string label = "";
        switch (InputSystem::getInstance()->getMosue()->getState((Mouse::Button)i)) {
        case ButtonState::None:
            label = "";
            break;
        case ButtonState::Trigger:
            label = "Trigger";
            break;
        case ButtonState::Pressed:
            label = "Pressed";
            break;
        case ButtonState::Released:
            label = "Released";
            break;
        }
        switch ((Mouse::Button)i) {
        case Mouse::Button::Left:
            ImGui::LabelText("MouseLeft", "%s", label.c_str());
            break;
        case Mouse::Button::Middle:
            ImGui::LabelText("MouseMiddle", "%s", label.c_str());
            break;
        case Mouse::Button::Right:
            ImGui::LabelText("MouseRight", "%s", label.c_str());
            break;
        default:
            break;
        }
    }
    auto mousePos = InputSystem::getInstance()->getMosue()->getPosition();
    ImGui::LabelText("MousePos", "%d, %d", mousePos.x(), mousePos.y());
    ImGui::End();
};
void DebugScene::onDraw(Renderer& renderer)
{
    renderer.position(m_cameraPos);
    renderer.lookAt(m_cameraLookAt);

    float previewTileW = static_cast<float>(getColorIndex() + m_tileSide);
    float previewBackTileW = static_cast<float>(getColorIndex() + TileBatch::s_tileReverseTable.at(m_tileSide));
    std::array<Vector4, 2> previewTiles {
        Vector4(m_tilePosition, previewTileW),
        Vector4(m_tilePosition + TileBatch::s_normalVectorTable.at(m_tileSide), previewBackTileW),
    };
    renderer.batchTileArray(TileBufferKind::UltraSmall, m_previewTileID, previewTiles.data(), 2);
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

void DebugScene::updateTile(Renderer& renderer)
{
    float w = static_cast<float>(getColorIndex() + m_tileSide);
    updateTileAt(m_tileData, m_tilePosition, m_tileSide, Vector4(m_tilePosition, w));
    renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tileData.data(), m_tileData.size());

    float backW = static_cast<float>(getColorIndex() + TileBatch::s_tileReverseTable.at(m_tileSide));
    Vector3 backTilePosition = m_tilePosition + TileBatch::s_normalVectorTable.at(m_tileSide);
    updateTileAt(m_backTileData, backTilePosition, TileBatch::s_tileReverseTable.at(m_tileSide), Vector4(backTilePosition, backW));
    renderer.batchTileArray(TileBufferKind::Medium, m_backTileID, m_backTileData.data(), m_backTileData.size());
}

void DebugScene::removeTile(Renderer& renderer)
{
    removeTileAt(m_tileData, m_tilePosition, m_tileSide);
    renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tileData.data(), m_tileData.size());

    Vector3 backTilePosition = m_tilePosition + TileBatch::s_normalVectorTable.at(m_tileSide);
    removeTileAt(m_backTileData, backTilePosition, TileBatch::s_tileReverseTable.at(m_tileSide));
    renderer.batchTileArray(TileBufferKind::Medium, m_backTileID, m_backTileData.data(), m_backTileData.size());
}

void DebugScene::putTile(Renderer& renderer)
{
    if (!containTileAt(m_tileData, m_tilePosition, m_tileSide)) {
        float w = static_cast<float>(getColorIndex() + m_tileSide);
        m_tileData.push_back(Vector4(m_tilePosition, w));
        renderer.batchTileArray(TileBufferKind::Medium, m_tileID, m_tileData.data(), m_tileData.size());

        float backW = static_cast<float>(getColorIndex() + TileBatch::s_tileReverseTable.at(m_tileSide));
        Vector3 backTilePosition = m_tilePosition + TileBatch::s_normalVectorTable.at(m_tileSide);
        m_backTileData.push_back(Vector4(backTilePosition, backW));
        renderer.batchTileArray(TileBufferKind::Medium, m_backTileID, m_backTileData.data(), m_backTileData.size());
        m_tileSubmit = true;
    }
}

bool DebugScene::containTileAt(std::vector<Vector4>& v, const Vector3& tilePosition, int32_t tileSide)
{
    auto iter = std::find_if(v.begin(), v.end(), [tilePosition, tileSide](const Vector4& tile) -> bool {
        return tilePosition == (Vector3)tile && (static_cast<int>(tile.w()) % 10) == tileSide;
    });
    return iter != v.end();
}

void DebugScene::updateTileAt(std::vector<Vector4>& v, const Vector3& tilePosition, int32_t tileSide, const Vector4& newTile)
{
    auto iter = std::find_if(v.begin(), v.end(), [tilePosition, tileSide](const Vector4& tile) -> bool {
        return tilePosition == (Vector3)tile && (static_cast<int>(tile.w()) % 10) == tileSide;
    });
    if (iter != v.end()) {
        *iter = newTile;
    }
}

void DebugScene::removeTileAt(std::vector<Vector4>& v, const Vector3& tilePosition, int32_t tileSide)
{
    auto iter = std::remove_if(v.begin(), v.end(), [tilePosition, tileSide](const Vector4& tile) -> bool {
        return tilePosition == (Vector3)tile && (static_cast<int>(tile.w()) % 10) == tileSide;
    });
    if (iter != v.end()) {
        v.erase(iter, v.end());
    }
}
}