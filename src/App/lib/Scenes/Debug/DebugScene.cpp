#include <Common/Constants.hpp>
#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// public
DebugScene::DebugScene()
    : m_tileRenderer()
    , m_tilePosition()
    , m_tileSide()
    , m_tilePallet()
    , m_tileColor()
    , m_cameraPos({ 0, 0, -3 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.08f)
    , m_cameraRotateSpeed(0.8f)
    , m_cursorVisible(true)
    , m_ioFile()
    , m_editCompleted()
    , m_tileID()
    , m_tiles()
    , m_hintTileID()
    , m_hintTiles()
    , m_hintTileBatch()
    , m_renderer()
{
    std::string fileName = "assets/model1.csv";
    std::copy(fileName.begin(), fileName.end(), m_ioFile.begin());
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter()
{
    auto tex = Texture::create("./assets/tileNormal2.png");
    if (!m_tileRenderer) {
        m_tileRenderer = std::make_shared<TileRenderer>(tex, Common::Constants::k_tileSize);
    }
    if (!m_hintTileBatch) {
        m_hintTileBatch = TileBatch::create(TileBufferUltraSmall::create(1), tex, Common::Constants::k_tileSize, TileBatch::Style::WireframeWithCross);
    }
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_debugTexture) {
        m_debugTexture = Texture::create("./assets/tileNormal2.png");
    }
    m_tileID = m_tileRenderer->rentTile(TileBufferKind::UltraLarge);
    m_hintTileID = m_hintTileBatch->rent();

    if (m_tiles.size() == 0) {
        m_tiles.push_back(Vector4({ 0, 0, 0, 0 }));
        m_tiles.push_back(Vector4({ 0, 0, 0, 1 }));
        m_tiles.push_back(Vector4({ 0, 0, 0, 2 }));
        m_tiles.push_back(Vector4({ 0, 0, 0, 3 }));
        m_tiles.push_back(Vector4({ 0, 0, 0, 4 }));
        m_tiles.push_back(Vector4({ 0, 0, 0, 5 }));
    }
    m_editCompleted = false;
    m_tileRenderer->batchTileArray(TileBufferKind::UltraLarge, m_tileID, m_tiles.data(), m_tiles.size());
}
void DebugScene::onExit()
{
    m_tileRenderer->releaseTile(TileBufferKind::UltraLarge, m_tileID);
    m_hintTileBatch->release(m_hintTileID);
}

void DebugScene::onUpdate()
{
    (void)m_cameraAngleX;
    (void)m_cameraAngleY;

    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();
    std::optional<Vector3> optHitPos = std::nullopt;
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

    optHitPos = scanHintTiles(forward);
    m_hintTileBatch->setTiles(m_hintTileID, m_hintTiles.data(), m_hintTiles.size());

    if (Cursor::isLocked()) {
        if (mouse->isTrigger(Mouse::Button::Right)) {
            for (const auto& hintTile : m_placeTiles) {
                m_tiles.push_back(hintTile);
            }
            compactTiles();
            m_tileRenderer->batchTileArray(TileBufferKind::UltraLarge, m_tileID, m_tiles.data(), m_tiles.size());
        } else if (mouse->isTrigger(Mouse::Button::Left)) {
            if (m_tiles.size() > 6 && optHitPos) {
                auto iter = std::remove_if(m_tiles.begin(), m_tiles.end(), [optHitPos](const auto& tile) -> bool {
                    return *optHitPos == (Vector3)tile;
                });
                m_tiles.erase(iter, m_tiles.end());
                restoreTiles(*optHitPos);

                m_tileRenderer->batchTileArray(TileBufferKind::UltraLarge, m_tileID, m_tiles.data(), m_tiles.size());
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

void DebugScene::onGui()
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
    ImGui::LabelText("TileCount", "%d", static_cast<int32_t>(m_tiles.size()));
    if (ImGui::Button("Exit")) {
        m_editCompleted = true;
    }
    ImGui::End();

    ImGui::Begin("Cursor");
    auto pos = InputSystem::getInstance()->getMouse()->getPosition();
    auto delta = InputSystem::getInstance()->getMouse()->getDelta();
    ImGui::LabelText("Pos", "%d, %d", pos.x(), pos.y());
    ImGui::LabelText("Delta", "%d, %d", delta.x(), delta.y());
    ImGui::End();

    ImGui::Begin("IO");
    ImGui::InputText("File", m_ioFile.data(), 32);
    if (ImGui::Button("Save")) {
        IO::serializeTile(std::string(m_ioFile.data()), m_tiles, Common::Constants::k_tileSize);
    }
    if (ImGui::Button("Load")) {
        IO::deserializeTile(std::string(m_ioFile.data()), m_tiles, Common::Constants::k_tileSize);
        compactTiles();
        m_tileRenderer->batchTileArray(TileBufferKind::UltraLarge, m_tileID, m_tiles.data(), m_tiles.size());
    }
    ImGui::End();
}
void DebugScene::onDraw3D()
{
    Camera::position(m_cameraPos);
    Camera::lookAt(m_cameraLookAt);

    m_tileRenderer->drawTiles();

    m_hintTileBatch->setGlobalViewMatrix(Camera::getLookAtMatrix());
    m_hintTileBatch->setGlobalProjectionMatrix(Camera::getPerspectiveMatrix());
    Engine::getInstance()->getDevice()->getSurface()->render(m_hintTileBatch);

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
    m_renderer->drawPlaneTexture(Vector3({ -10, 0, 10 }), Vector2({ 10, 10 }), q, m_debugTexture, Vector4({ 1, 1, 1, 1 }));
    m_renderer->drawBox(Vector3({ 25, 0, 10 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawBox(Vector3({ 25, 0, 20 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), true);
    m_renderer->drawBoxTexture(Vector3({ -25, 0, 10 }), Vector3({ 5, 5, 5 }), q, m_debugTexture, Vector4({ 1, 1, 1, 1 }));
}

void DebugScene::onDraw2D()
{
    m_renderer->drawCircle(Vector2({ 0, 0 }), Vector2({ 50, 50 }), Vector4({ 1, 1, 1, 1 }));
    m_renderer->drawRect(Vector2({ 100, 100 }), Vector2({ 50, 50 }), 45.0f, Vector4({ 1, 0, 0, 1 }));
    m_renderer->drawSprite(Vector2({ -100, -100 }), Vector2({ 100, 100 }), 0.0f, m_debugTexture, Vector4({ 1, 1, 1, 1 }));
    m_renderer->textFont(FontFactory::getInstance()->load("./assets/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ 200, 200 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"こんにちわ", Vector4({ 1, 1, 1, 1 }));
}

bool DebugScene::tryTransition(std::string& outNextScene)
{
    if (m_editCompleted) {
        outNextScene = "Game";
    }
    return m_editCompleted;
}
// private
int32_t DebugScene::getColorIndex() const
{
    return ((m_tilePallet * 16) + m_tileColor) * 10;
}

void DebugScene::compactTiles()
{
    for (auto& tile : m_tiles) {
        if (tile.w() < 0.0f) {
            continue;
        }
        int32_t tileSide = static_cast<int32_t>(tile.w()) % 10;
        int32_t backSide = TileBatch::k_tileReverseTable.at(tileSide);
        Vector3 normal = TileBatch::k_normalVectorTable.at(tileSide);
        Vector3 backPos = (Vector3)tile + (normal * Vector3({ Common::Constants::k_tileSize, Common::Constants::k_tileSize, Common::Constants::k_tileSize }));

        for (auto& otherTile : m_tiles) {
            if ((Vector3)otherTile == backPos && static_cast<int32_t>(otherTile.w()) % 10 == backSide) {
                otherTile.w() = -1;
                tile.w() = -1;
            }
        }
    }
    auto iter = std::remove_if(m_tiles.begin(), m_tiles.end(), [](const auto& tile) -> bool {
        return tile.w() < 0.0f;
    });
    m_tiles.erase(iter, m_tiles.end());
}

void DebugScene::restoreTiles(const Vector3& center)
{
    for (const auto& normal : TileBatch::k_normalVectorTable) {
        auto sidePos = center + (normal * Vector3({ Common::Constants::k_tileSize, Common::Constants::k_tileSize, Common::Constants::k_tileSize }));

        std::vector<Vector4> missingVec;
        for (int32_t side = 0; side < 6; side++) {
            auto iter = std::find_if(m_tiles.begin(), m_tiles.end(), [sidePos, side](const auto& otherTile) -> bool {
                int32_t w = static_cast<int32_t>(otherTile.w());
                return (Vector3)otherTile == sidePos && side == (w % 10);
            });
            if (iter == m_tiles.end()) {
                missingVec.push_back(Vector4(sidePos, side));
            }
        }
        if (missingVec.size() != 6) {
            for (const auto& missingTile : missingVec) {
                m_tiles.push_back(missingTile);
            }
        }
    }
}

std::optional<Vector3> DebugScene::scanHintTiles(Vector3 forward)
{
    m_hintTiles.clear();
    m_placeTiles.clear();

    auto hitTiles = Raycast::testTiles(m_cameraPos, forward, 10.0f, Common::Constants::k_tileSize);
    std::optional<Vector3> optHitPos = std::nullopt;
    for (const auto& hitTile : hitTiles) {
        for (const auto& otherTile : m_tiles) {
            if (hitTile.position == (Vector3)otherTile) {
                auto iter = std::find(TileBatch::k_normalVectorTable.begin(), TileBatch::k_normalVectorTable.end(), hitTile.normal);
                int32_t hitSide = std::distance(TileBatch::k_normalVectorTable.begin(), iter);
                int32_t tileSide = static_cast<int32_t>(otherTile.w()) % 10;
                if (hitSide == tileSide) {
                    auto placePos = hitTile.position + (hitTile.normal * Vector3({ Common::Constants::k_tileSize, Common::Constants::k_tileSize, Common::Constants::k_tileSize }));
                    bool found = std::find_if(m_tiles.begin(), m_tiles.end(), [placePos](const auto& e) -> bool {
                        return placePos == (Vector3)e;
                    }) != m_tiles.end();
                    if (!found) {
                        optHitPos = hitTile.position;
                        m_placeTiles.push_back(Vector4(placePos, 0 + getColorIndex()));
                        m_placeTiles.push_back(Vector4(placePos, 1 + getColorIndex()));
                        m_placeTiles.push_back(Vector4(placePos, 2 + getColorIndex()));
                        m_placeTiles.push_back(Vector4(placePos, 3 + getColorIndex()));
                        m_placeTiles.push_back(Vector4(placePos, 4 + getColorIndex()));
                        m_placeTiles.push_back(Vector4(placePos, 5 + getColorIndex()));

                        auto hintPos = hitTile.position + (hitTile.normal * (Common::Constants::k_tileSize / 10.0f));
                        m_hintTiles.push_back(Vector4(hintPos, 630 + hitSide));
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