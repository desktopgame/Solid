#include <Common/Graphics/NodeRegistry.hpp>
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
{
}
GameScene::~GameScene() { }

void GameScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_field) {
        auto normalTex = Texture::create("./assets/tileNormal2.png");
        auto borderTex = Texture::create("./assets/tileBorder.png");
        m_field = std::make_shared<System::Field>(normalTex, borderTex);
        m_field->load("assets\\Stages\\stage_base.csv");

        m_debugEntity = System::Entities::SlimeEntity::create(Common::Graphics::NodeRegistry::s_slimeNode->clone());
        m_debugEntity->setPosition(Vector3({ 90, 20, 90 }));
        m_field->spwan(m_debugEntity);

        m_debugPlayer = System::Entities::PlayerEntity::create(
            Common::Graphics::Node::deserialize("assets\\Models\\player.json"));
        m_debugPlayer->setPosition(Vector3({ 80, 20, 80 }));
        m_field->setPlayer(m_debugPlayer);
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
    ImGui::Begin("Light");
    ImGui::DragFloat3("GlobalLight", m_globalLightDir.data(), 0.001f, -1.0f, 1.0f);
    ImGui::End();

    ImGui::Begin("Entity");
    Vector3 vel = m_debugEntity->getVelocity();
    if (ImGui::DragFloat3("Velocity", vel.data(), 0.01f)) {
        m_debugEntity->setVelocity(vel);
    }
    Vector3 pos = m_debugEntity->getPosition();
    if (ImGui::DragFloat3("Position", pos.data(), 0.01f)) {
        m_debugEntity->setPosition(pos);
    }
    if (ImGui::Button("Jump")) {
        m_debugEntity->setVelocity(Vector3({ 0, 100, 0 }));
    }

    ImGui::End();
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
}

void GameScene::onDraw2D()
{
    m_field->draw2D(m_renderer);
}

bool GameScene::tryTransition(std::string& outNextScene)
{
    return false;
}
// private
}