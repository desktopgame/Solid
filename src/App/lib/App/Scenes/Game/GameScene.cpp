#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Common/Graphics/ParticleSystem.hpp>
#include <App/Common/Graphics/TelopSystem.hpp>
#include <App/Scenes/Game/GameScene.hpp>
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>
#include <App/Scenes/Game/System/Entities/SlimeEntity.hpp>
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
    , m_minimap()
#if GAMESCENE_PROFILE
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

        auto chunk = m_field->getCurrentChunk();
        m_debugPlayer = System::Entities::PlayerEntity::create(Common::Graphics::Node::deserialize("./assets/Models/Player.json"));
        m_debugPlayer->setPosition(Vector3({ chunk->getPhysicalCenterX(), 10, chunk->getPhysicalCenterZ() }));
        m_field->setPlayer(m_debugPlayer);
    }
    if (!m_aimTexture) {
        m_aimTexture = Texture::create("./assets/Textures/aim.png");
    }
    if (!m_minimap) {
        auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");

        m_minimap = RootPane::create();

        auto title = std::make_shared<Label>();
        title->setFont(font);
        title->setText(u"Minimap");
        title->setPreferredSize(font->measure(title->getFontSize(), title->getText()) + Vector2({ 0, 10 }));
        m_minimap->addLayoutElement(std::make_shared<LayoutElement>(title, std::make_shared<BorderLayout::Hint>("TOP")));
        m_minimap->addLayoutElement(std::make_shared<LayoutElement>(std::make_shared<System::Minimap>(m_field), std::make_shared<BorderLayout::Hint>("CENTER")));

        m_minimap->setSize(Vector2({ 160, 190 }));
        m_minimap->setPosition(((Vector2)Screen::getSize() / 2.0f) - (m_minimap->getSize() / 2.0f));

        m_minimap->doLayout();
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
    m_minimap->update();
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
    m_minimap->draw2D(m_renderer);
    Common::Graphics::TelopSystem::draw();
    m_renderer->drawSprite(Vector2({ 0, 0 }), Vector2({ 32, 32 }), 0.0f, m_aimTexture, Vector4({ 1, 1, 1, 1 }));

#if GAMESCENE_PROFILE
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