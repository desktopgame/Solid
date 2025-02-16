#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Common/Graphics/ParticleSystem.hpp>
#include <App/Common/Graphics/TelopSystem.hpp>
#include <App/Scenes/Game/GameScene.hpp>
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>
#include <App/Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <App/Scenes/Game/UI/Piece.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
    : m_globalLightDir({ 1, 1, 0 })
    , m_renderer()
    , m_fontMap()
    , m_field()
    , m_debugPlayer()
    , m_debugEntity()
    , m_aimTexture()
    , m_minimap()
    , m_pauseUI()
    , m_requestPauseClose()
    , m_nextScene()
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
    if (!m_fontMap) {
        m_fontMap = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
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
        m_minimap->addLayoutElement(std::make_shared<LayoutElement>(std::make_shared<UI::Minimap>(m_field), std::make_shared<BorderLayout::Hint>("CENTER")));

        m_minimap->setSize(Vector2({ 160, 190 }));
        m_minimap->setPosition(((Vector2)Screen::getSize() / 2.0f) - (m_minimap->getSize() / 2.0f));

        m_minimap->doLayout();
    }
    if (!m_pauseUI) {
        m_pauseUI = RootPane::create();
        m_pauseUI->setPosition(Vector2({ 0, 0 }));
        m_pauseUI->setSize(Vector2({ 600, 400 }));

        auto tabbedPane = std::make_shared<TabbedPane>();
        tabbedPane->setFont(m_fontMap);

        auto tabStatus = std::make_shared<Panel>();
        tabStatus->setLayout(std::make_shared<BorderLayout>());
        tabStatus->setFlexible(true);
        auto statusForm = std::make_shared<Form>(m_fontMap, 16, Vector2({ 150, 30 }));
        statusForm->addLabel(u"Maximum-HP", u"100");
        statusForm->addLabel(u"ATK", u"100");
        statusForm->addLabel(u"DEF", u"100");
        statusForm->addLabel(u"SPD", u"100");
        tabStatus->addLayoutElement(std::make_shared<LayoutElement>(statusForm, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tabStatus, nullptr));
        tabbedPane->setTitleAt(0, u"STATUS");

        auto tabMap = std::make_shared<Panel>();
        tabMap->setLayout(std::make_shared<BorderLayout>());
        tabMap->setFlexible(true);
        m_map = std::make_shared<UI::Map>(m_field);
        auto mapScroll = std::make_shared<ScrollPane>();
        mapScroll->setView(m_map);
        auto list = std::make_shared<List<std::u16string>>();
        auto listCellRenderer = std::make_shared<StringListCellRenderer>();
        listCellRenderer->setFont(m_fontMap);
        listCellRenderer->setPreferredSize(Vector2({ 100, 50 }));
        list->setCellRenderer(listCellRenderer);
        list->addItem(u"Hello1");
        list->addItem(u"Hello2");
        list->addItem(u"Hello3");
        list->addItem(u"Hello4");
        list->addItem(u"Hello5");
        list->addItem(u"Hello6");
        list->addItem(u"Hello7");
        list->addItem(u"Hello8");
        list->addItem(u"Hello9");
        list->addItem(u"Hello10");
        list->setFlexible(true);
        auto listScroll = std::make_shared<ScrollPane>();
        listScroll->setView(list);
        auto sideBar = std::make_shared<Panel>();
        sideBar->setFlexible(true);
        sideBar->setLayout(std::make_shared<BorderLayout>());
        sideBar->setBackgroundColor(Color({ 1.0f, 1.0f, 0, 1.0f }));
        sideBar->addLayoutElement(std::make_shared<LayoutElement>(listScroll, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabMap->addLayoutElement(std::make_shared<LayoutElement>(mapScroll, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabMap->addLayoutElement(std::make_shared<LayoutElement>(sideBar, std::make_shared<BorderLayout::Hint>("RIGHT")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tabMap, nullptr));
        tabbedPane->setTitleAt(1, u"MAP");

        auto tabSystem = std::make_shared<Panel>();
        tabSystem->setLayout(std::make_shared<BorderLayout>());
        tabSystem->setFlexible(true);

        auto contentPanel = std::make_shared<Panel>();
        contentPanel->setBackgroundColor(Color({ 0.5f, 0.5f, 0.5f, 1.0f }));
        contentPanel->setLayout(std::make_shared<BorderLayout>());
        contentPanel->setFlexible(true);

        auto returnButtonBox = Box::createVerticalBox();
        auto returnButton = std::make_shared<Button>();
        returnButton->setFont(m_fontMap);
        returnButton->setText(u"EXIT");
        returnButton->setPreferredSize(Vector2({ 200, 40 }));
        returnButton->setOnClick(std::bind(&GameScene::onClickExitButton, this));
        returnButtonBox->addLayoutElement(std::make_shared<LayoutElement>(Box::createVerticalGlue(), std::make_shared<BorderLayout::Hint>("BOTTOM")));
        returnButtonBox->addLayoutElement(std::make_shared<LayoutElement>(returnButton, std::make_shared<BorderLayout::Hint>("BOTTOM")));
        returnButtonBox->addLayoutElement(std::make_shared<LayoutElement>(Box::createVerticalGlue(), std::make_shared<BorderLayout::Hint>("BOTTOM")));
        tabSystem->addLayoutElement(std::make_shared<LayoutElement>(contentPanel, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabSystem->addLayoutElement(std::make_shared<LayoutElement>(returnButtonBox, std::make_shared<BorderLayout::Hint>("BOTTOM")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tabSystem, nullptr));
        tabbedPane->setTitleAt(2, u"SYSTEM");

        auto closeButton = std::make_shared<Button>();
        closeButton->setFont(m_fontMap);
        closeButton->setText(u"CLOSE");
        closeButton->setPreferredSize(Vector2({ 200, 40 }));
        closeButton->setOnClick(std::bind(&GameScene::onClickPauseClose, this));

        m_pauseUI->addLayoutElement(std::make_shared<LayoutElement>(tabbedPane, std::make_shared<BorderLayout::Hint>("CENTER")));
        m_pauseUI->addLayoutElement(std::make_shared<LayoutElement>(closeButton, std::make_shared<BorderLayout::Hint>("BOTTOM")));
    }
    if (Cursor::isVisible()) {
        Cursor::hide();
        Cursor::lock(Engine::getInstance()->getWindow());
    }
    m_nextScene = "";
}

void GameScene::onExit()
{
}

void GameScene::onUpdate()
{
    auto keyboard = InputSystem::getInstance()->getKeyboard();

    if (keyboard->isTrigger(KeyCode::E) || m_requestPauseClose) {
        if (Cursor::isVisible()) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();

            m_map->setup();
            m_pauseUI->doLayout();
        }
        m_requestPauseClose = false;
    }

    if (Cursor::isVisible()) {
        m_pauseUI->update();
    } else {
        m_field->update();
        m_minimap->update();
    }
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

    if (Cursor::isVisible()) {
        m_pauseUI->draw2D(m_renderer);
    }

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
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
void GameScene::onClickPauseClose()
{
    m_requestPauseClose = true;
}
void GameScene::onClickExitButton()
{
#if _DEBUG
    m_nextScene = "Debug";
#else
    m_nextScene = "Title";
#endif
}
}