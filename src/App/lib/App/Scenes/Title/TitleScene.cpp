#include <App/Scenes/Title/TitleScene.hpp>

namespace App::Scenes::Title {
// public
TitleScene::TitleScene()
    : m_renderer()
    , m_fontMap()
    , m_baseUI()
    , m_nextScene()
{
}
TitleScene::~TitleScene()
{
}

void TitleScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_fontMap) {
        m_fontMap = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
    }
    if (!m_baseUI) {
        m_baseUI = RootPane::create();
        m_baseUI->setPosition(Vector2({ 0, 0 }));
        m_baseUI->setSize((Vector2)Screen::getSize());

        auto title = std::make_shared<Label>();
        title->setFont(m_fontMap);
        title->setFontSize(32);
        title->setText(u"SOLID");
        title->setForegroundColor(Color({ 0, 1, 0, 1 }));
        title->setPreferredSize(m_fontMap->measure(title->getFontSize(), title->getText()) + Vector2({ 0, 100 }));
        m_baseUI->addLayoutElement(std::make_shared<LayoutElement>(title, std::make_shared<BorderLayout::Hint>("TOP")));

        auto menu = Box::createVerticalBox();
        menu->addLayoutElement(std::make_shared<LayoutElement>(Box::createVerticalGlue(), nullptr));

        auto startButton = std::make_shared<Button>();
        startButton->setFont(m_fontMap);
        startButton->setFontSize(32);
        startButton->setText(u"START");
        startButton->setPreferredSize(Vector2({ 200, 50 }));
        startButton->setOnClick(std::bind(&TitleScene::onClickStartButton, this));
        menu->addLayoutElement(std::make_shared<LayoutElement>(startButton, nullptr));

        auto settingButton = std::make_shared<Button>();
        settingButton->setFont(m_fontMap);
        settingButton->setFontSize(32);
        settingButton->setText(u"SETTING");
        settingButton->setPreferredSize(Vector2({ 200, 50 }));
        settingButton->setOnClick(std::bind(&TitleScene::onClickSettingButton, this));
        menu->addLayoutElement(std::make_shared<LayoutElement>(settingButton, nullptr));

        menu->addLayoutElement(std::make_shared<LayoutElement>(Box::createVerticalGlue(), nullptr));
        m_baseUI->addLayoutElement(std::make_shared<LayoutElement>(menu, std::make_shared<BorderLayout::Hint>("CENTER")));

        m_baseUI->doLayout();
    }
    m_nextScene = "";
}
void TitleScene::onExit() { }

void TitleScene::onUpdate()
{
    m_baseUI->update();
}
void TitleScene::onGui() { }
void TitleScene::onDraw3D() { }
void TitleScene::onDraw2D()
{
    m_baseUI->draw2D(m_renderer);
}

bool TitleScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
void TitleScene::onClickStartButton()
{
    m_nextScene = "Game";
}
void TitleScene::onClickSettingButton() { }
}