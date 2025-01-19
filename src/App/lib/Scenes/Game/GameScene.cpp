#include <Common/Graphics/NodeRegistry.hpp>
#include <Common/Graphics/ParticleSystem.hpp>
#include <Common/Graphics/TelopSystem.hpp>
#include <Scenes/Game/GameScene.hpp>
#include <Scenes/Game/System/Entities/BasicEntity.hpp>
#include <Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
    : m_globalLightDir({ 1, 1, 0 })
    , m_renderer()
    , m_field()
    , m_debugPlayer()
    , m_debugEntity()
    , m_aimTexture()
#if _DEBUG
    , m_avgTime()
#endif
{
}
GameScene::~GameScene() { }

void GameScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_field) {
        auto normalTex = Texture::create("./assets/Textures/tileNormal2.png");
        auto borderTex = Texture::create("./assets/Textures/tileBorder.png");
        m_field = std::make_shared<System::Field>(normalTex, borderTex);
        m_field->generate();

        Random rand;
        int32_t mainRoomIndex = rand.range(0, m_field->getRoomCount() - 1);

        for (int32_t i = 0; i < m_field->getRoomCount(); i++) {
            if (i == mainRoomIndex) {
                continue;
            }
            System::FieldGenerator::Room room = m_field->getRoomAt(i);
            for (int32_t j = 0; j < rand.range(1, 5); j++) {
                // int32_t halfX = (System::Field::k_roomSizeX / 2) - 3;
                // int32_t halfZ = (System::Field::k_roomSizeZ / 2) - 3;
                // int32_t tileOffsetX = rand.range(-halfX, halfX);
                // int32_t tileOffsetZ = rand.range(-halfZ, halfZ);

                float enemyPosX = (room.center.x() + 0) * System::Field::k_tileSize;
                float enemyPosZ = (room.center.z() + 0) * System::Field::k_tileSize;

                m_debugEntity = System::Entities::SlimeEntity::create();
                m_debugEntity->setPosition(Vector3({ enemyPosX, 10, enemyPosZ }));
                m_field->spwan(m_debugEntity);
            }
        }

        System::FieldGenerator::Room mainRoom = m_field->getRoomAt(mainRoomIndex);
        m_debugPlayer = System::Entities::PlayerEntity::create(
            Common::Graphics::Node::deserialize("./assets/Models/Player.json"));
        m_debugPlayer->setPosition(Vector3({ mainRoom.center.x() * System::Field::k_tileSize, 10, mainRoom.center.z() * System::Field::k_tileSize }));
        m_field->setPlayer(m_debugPlayer);
    }
    if (!m_aimTexture) {
        m_aimTexture = Texture::create("./assets/Textures/aim.png");
    }
    if (Cursor::isVisible()) {
        Cursor::hide();
        Cursor::lock(Engine::getInstance()->getWindow());
    }
}

void GameScene::onExit()
{
}

void GameScene::onUpdate()
{
    m_field->update();
}

void GameScene::onGui()
{
    m_field->onGui();
}

void GameScene::onDraw3D()
{
    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(m_globalLightDir));

    PointLight::enable();
    PointLight::setCount(3);
    PointLight::set(0, Vector3({ 120, 10, 120 }), 10, 20);
    PointLight::set(1, Vector3({ 90, 20, 90 }), 20, 30);
    PointLight::set(2, Vector3({ 60, 10, 60 }), 10, 20);

    m_field->draw3D(m_renderer);
    Common::Graphics::ParticleSystem::draw();
}

void GameScene::onDraw2D()
{
    m_field->draw2D(m_renderer);
    Common::Graphics::TelopSystem::draw();
    m_renderer->drawSprite(Vector2({ 0, 0 }), Vector2({ 32, 32 }), 0.0f, m_aimTexture, Vector4({ 1, 1, 1, 1 }));

#if _DEBUG
    float dt = Time::deltaTime();
    m_avgTime *= 1.0f - k_fpsK;
    m_avgTime += dt * k_fpsK;
    float fps = 1.0 / m_avgTime;
    std::string fpsT = std::to_string(fps);
    std::u16string uFpsT = std::u16string(fpsT.begin(), fpsT.end());
    m_renderer->textFont(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ 0, 100 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, uFpsT, Color({ 1, 0, 0, 1 }));
#endif
}

bool GameScene::tryTransition(std::string& outNextScene)
{
    return false;
}
// private
}