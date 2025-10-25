#include <App/Common/Audio/AudioManager.hpp>
#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Common/Graphics/ParticleSystem.hpp>
#include <App/Common/Graphics/TelopSystem.hpp>
#include <App/Scenes/Game/GameScene.hpp>
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>
#include <App/Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <App/Scenes/Game/System/PieceRegistry.hpp>
#include <App/Scenes/Game/UI/Piece.hpp>
#include <App/Scenes/Game/UI/PieceInfoListCellRenderer.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
    : m_globalLightDir({ 1, 1, 0 })
    , m_renderer()
    , m_fontMap()
    , m_field()
    , m_pieceInstanceCollection()
    , m_debugPlayer()
    , m_debugEntity()
    , m_aimTexture()
    , m_minimap()
    , m_pauseUI()
    , m_weaponUI()
    , m_mainWeaponIcon()
    , m_mainWeaponIconPath()
    , m_mainWeaponEnergyGauge()
    , m_subWeaponIcon()
    , m_subWeaponIconPath()
    , m_subWeaponEnergyGauge()
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
    if (!m_pieceInstanceCollection) {
        m_pieceInstanceCollection = std::make_shared<System::PieceInstanceCollection>();
    }
    if (!m_aimTexture) {
        m_aimTexture = Texture::create("./assets/Textures/aim.png");
    }
    if (!m_minimap) {
        auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");

        m_minimap = RootPane::create();

        auto minimapChild = std::make_shared<UI::Minimap>(m_field);
        minimapChild->setPieceInstanceCollection(m_pieceInstanceCollection);

        auto title = std::make_shared<Label>();
        title->setFont(font);
        title->setText(u"Minimap");
        title->setPreferredSize(font->measure(title->getFontSize(), title->getText()) + Vector2({ 0, 10 }));
        m_minimap->addLayoutElement(std::make_shared<LayoutElement>(title, std::make_shared<BorderLayout::Hint>("TOP")));
        m_minimap->addLayoutElement(std::make_shared<LayoutElement>(minimapChild, std::make_shared<BorderLayout::Hint>("CENTER")));

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

        auto tabSystem = std::make_shared<Panel>();
        tabSystem->setLayout(std::make_shared<BorderLayout>());
        tabSystem->setFlexible(true);

        auto contentPanel = std::make_shared<Panel>();
        // contentPanel->setBackgroundColor(Color({ 0.5f, 0.5f, 0.5f, 1.0f }));
        contentPanel->setLayout(std::make_shared<BorderLayout>());
        contentPanel->setFlexible(true);

        auto contentLayout = std::make_shared<BoxLayout>(BoxLayout::Orientation::Vertical);
        contentPanel->setLayout(contentLayout);
        // 【操作方法】
        {
            auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
            auto label = std::make_shared<Label>();
            label->setFont(font);
            label->setText(u"【操作方法】");
            label->setPreferredSize(font->measure(label->getFontSize(), label->getText()));

            auto alignLeft = Box::createHorizontalBox();
            alignLeft->addLayoutElement(std::make_shared<LayoutElement>(label, nullptr));
            alignLeft->addLayoutElement(std::make_shared<LayoutElement>(Box::createHorizontalGlue(), nullptr));

            contentPanel->addLayoutElement(std::make_shared<LayoutElement>(alignLeft, nullptr));
        }
        // WASDキー：移動
        {
            auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
            auto label = std::make_shared<Label>();
            label->setFont(font);
            label->setText(u"WASDキー：移動");
            label->setPreferredSize(font->measure(label->getFontSize(), label->getText()));

            auto alignLeft = Box::createHorizontalBox();
            alignLeft->addLayoutElement(std::make_shared<LayoutElement>(label, nullptr));
            alignLeft->addLayoutElement(std::make_shared<LayoutElement>(Box::createHorizontalGlue(), nullptr));

            contentPanel->addLayoutElement(std::make_shared<LayoutElement>(alignLeft, nullptr));
        }
        // Eキー：ポーズ
        {
            auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
            auto label = std::make_shared<Label>();
            label->setFont(font);
            label->setText(u"Eキー：ポーズ");
            label->setPreferredSize(font->measure(label->getFontSize(), label->getText()));

            auto alignLeft = Box::createHorizontalBox();
            alignLeft->addLayoutElement(std::make_shared<LayoutElement>(label, nullptr));
            alignLeft->addLayoutElement(std::make_shared<LayoutElement>(Box::createHorizontalGlue(), nullptr));

            contentPanel->addLayoutElement(std::make_shared<LayoutElement>(alignLeft, nullptr));
        }

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
        tabbedPane->setTitleAt(0, u"SYSTEM");

        auto closeButton = std::make_shared<Button>();
        closeButton->setFont(m_fontMap);
        closeButton->setText(u"CLOSE");
        closeButton->setPreferredSize(Vector2({ 200, 40 }));
        closeButton->setOnClick(std::bind(&GameScene::onClickPauseClose, this));

        m_pauseUI->addLayoutElement(std::make_shared<LayoutElement>(tabbedPane, std::make_shared<BorderLayout::Hint>("CENTER")));
        m_pauseUI->addLayoutElement(std::make_shared<LayoutElement>(closeButton, std::make_shared<BorderLayout::Hint>("BOTTOM")));
    }
    if (!m_weaponUI) {
        m_weaponUI = RootPane::create();
        m_weaponUI->setSize(Vector2({ 300, 200 }));
        m_weaponUI->setPosition(Vector2({ //
            static_cast<float>(Screen::getWidth()) / 2.0f - (m_weaponUI->getSize().x() / 2.0f),
            -static_cast<float>(Screen::getHeight()) / 2.0f + (m_weaponUI->getSize().y() / 2.0f) }));

        m_weaponUI->setLayout(std::make_shared<BoxLayout>(BoxLayout::Orientation::Vertical));

        auto mainWeaponHBox = Box::createHorizontalBox();
        m_mainWeaponIcon = std::make_shared<ImageIcon>();
        m_mainWeaponIcon->setPreferredSize(Vector2({ 80, 80 }));
        m_mainWeaponEnergyGauge = std::make_shared<Gauge>();
        m_mainWeaponEnergyGauge->setPreferredSize(Vector2({ 100, 30 }));
        m_mainWeaponEnergyGauge->setBackgroundColor(Vector4({ 0.5f, 0.1f, 0.1f, 1.0f }));
        m_mainWeaponEnergyGauge->setForegroundColor(Vector4({ 0.1f, 0.5f, 0.1f, 1.0f }));
        m_mainWeaponEnergyGauge->setFlexible(true);
        mainWeaponHBox->addLayoutElement(std::make_shared<LayoutElement>(m_mainWeaponIcon, nullptr));
        mainWeaponHBox->addLayoutElement(std::make_shared<LayoutElement>(m_mainWeaponEnergyGauge, nullptr));

        auto subWeaponHBox = Box::createHorizontalBox();
        m_subWeaponIcon = std::make_shared<ImageIcon>();
        m_subWeaponIcon->setPreferredSize(Vector2({ 80, 80 }));
        m_subWeaponEnergyGauge = std::make_shared<Gauge>();
        m_subWeaponEnergyGauge->setPreferredSize(Vector2({ 100, 30 }));
        m_subWeaponEnergyGauge->setBackgroundColor(Vector4({ 0.5f, 0.1f, 0.1f, 1.0f }));
        m_subWeaponEnergyGauge->setForegroundColor(Vector4({ 0.1f, 0.5f, 0.1f, 1.0f }));
        m_subWeaponEnergyGauge->setFlexible(true);
        subWeaponHBox->addLayoutElement(std::make_shared<LayoutElement>(m_subWeaponIcon, nullptr));
        subWeaponHBox->addLayoutElement(std::make_shared<LayoutElement>(m_subWeaponEnergyGauge, nullptr));

        m_weaponUI->addLayoutElement(std::make_shared<LayoutElement>(mainWeaponHBox, nullptr));
        m_weaponUI->addLayoutElement(std::make_shared<LayoutElement>(subWeaponHBox, nullptr));

        m_weaponUI->doLayout();
        // ウェポン変わってたら更新
        loadWeaponIcon();
        updateWeaponGauge();
    }
    if (Cursor::isVisible()) {
        Cursor::hide();
        Cursor::lock(Engine::getInstance()->getWindow());
    }
    m_nextScene = "";
    Common::Audio::AudioManager::getInstance()->playBgm("assets/Audios/bgm_sample01.wav");
}

void GameScene::onExit()
{
    Common::Audio::AudioManager::getInstance()->stopBgm();
}

void GameScene::onUpdate()
{
    auto keyboard = InputSystem::getInstance()->getKeyboard();

    if (keyboard->isTrigger(KeyCode::E) || m_requestPauseClose) {
        if (Cursor::isVisible()) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
            Time::s_timeScale = 1.0f;
        } else {
            Cursor::show();
            Cursor::unlock();
            Time::s_timeScale = 0.0f;

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

    // ウェポン変わってたら更新
    loadWeaponIcon();
    updateWeaponGauge();
}

void GameScene::onGui()
{
    m_field->onGui();
}

void GameScene::onDraw3D()
{
    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(m_globalLightDir));

    PointLight::disable();

    m_field->draw3D(m_renderer);
    Common::Graphics::ParticleSystem::draw();
}

void GameScene::onDraw2D()
{
    m_field->draw2D(m_renderer);
    m_minimap->draw2D(m_renderer);
    m_weaponUI->draw2D(m_renderer);
    Common::Graphics::TelopSystem::draw();
    m_renderer->drawSprite(Vector2({ 0, 0 }), Vector2({ 32, 32 }), 0.0f, m_aimTexture, Vector4({ 1, 1, 1, 1 }));

    if (Cursor::isVisible()) {
        m_pauseUI->draw2D(m_renderer);
    }

#if GAMESCENE_PROFILE
    float dt = Time::s_deltaTime;
    m_avgTime *= 1.0f - k_fpsK;
    m_avgTime += dt * k_fpsK;
    float fps = 1.0 / m_avgTime;
    std::string fpsT = std::to_string(fps);
    std::u16string uFpsT = std::u16string(fpsT.begin(), fpsT.end());
    m_renderer->textFont(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ -Screen::getWidth() / 2.0f, Screen::getHeight() / 2.0f }), Renderer::TextAlignX::Left, Renderer::TextAlignY::Top, uFpsT, Color({ 1, 0, 0, 1 }));
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
void GameScene::loadWeaponIcon()
{
    if (m_debugPlayer->getMainWeapon()) {
        auto path = m_debugPlayer->getMainWeapon()->getIconPath();
        if (m_mainWeaponIconPath != path) {
            m_mainWeaponIcon->setTexture(Texture::create(path));
            m_mainWeaponIconPath = path;
        }
    }
    if (m_debugPlayer->getSubWeapon()) {
        auto path = m_debugPlayer->getSubWeapon()->getIconPath();
        if (m_subWeaponIconPath != path) {
            m_subWeaponIcon->setTexture(Texture::create(path));
            m_subWeaponIconPath = path;
        }
    }
}

void GameScene::updateWeaponGauge()
{
    if (m_debugPlayer->getMainWeapon()) {
        m_mainWeaponEnergyGauge->setMaximumValue(m_debugPlayer->getMainWeapon()->getEnergyMax());
        m_mainWeaponEnergyGauge->setValue(m_debugPlayer->getMainWeaponEnergy());
    }
    if (m_debugPlayer->getSubWeapon()) {
        m_subWeaponEnergyGauge->setMaximumValue(m_debugPlayer->getSubWeapon()->getEnergyMax());
        m_subWeaponEnergyGauge->setValue(m_debugPlayer->getSubWeaponEnergy());
    }
}
}