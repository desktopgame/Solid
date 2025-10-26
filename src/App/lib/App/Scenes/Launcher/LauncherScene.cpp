#include <App/Scenes/Launcher/LauncherScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Launcher {
// public
LauncherScene::LauncherScene()
    : m_nextScene()
    , m_rootPane()
    , m_renderer()
{
}
LauncherScene::~LauncherScene() { }

void LauncherScene::onEnter()
{
    if (!m_rootPane) {
        auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");

        m_rootPane = RootPane::create();
        m_rootPane->setPosition(Vector2({ 0, 0 }));
        m_rootPane->setSize(Vector2({ 800, 600 }));

        auto tabbedPane = std::make_shared<TabbedPane>();
        tabbedPane->setFont(font);

        auto tabHome = std::make_shared<Panel>();
        tabHome->setLayout(std::make_shared<BorderLayout>());
        tabHome->setFlexible(true);
        {
            auto vbox = Box::createVerticalBox();

            auto description1 = std::make_shared<Label>();
            description1->setFont(font);
            description1->setText(u"このアプリケーションは自作の描画エンジンであるSolidの開発者向けデモです。");
            description1->setPreferredSize(font->measure(description1->getFontSize(), description1->getText()));
            vbox->addLayoutElement(std::make_shared<LayoutElement>(description1, nullptr));

            auto description2 = std::make_shared<Label>();
            description2->setFont(font);
            description2->setText(u"操作にはマウスとキーボードを用います。");
            description2->setPreferredSize(font->measure(description2->getFontSize(), description2->getText()));
            vbox->addLayoutElement(std::make_shared<LayoutElement>(description2, nullptr));

            auto description3 = std::make_shared<Label>();
            description3->setFont(font);
            description3->setText(u"画面上部ボタンをクリックすると表示内容が切り替わります。");
            description3->setPreferredSize(font->measure(description3->getFontSize(), description3->getText()));
            vbox->addLayoutElement(std::make_shared<LayoutElement>(description3, nullptr));

            tabHome->addLayoutElement(std::make_shared<LayoutElement>(vbox, std::make_shared<BorderLayout::Hint>("CENTER")));
        }

        auto tabScene = std::make_shared<Panel>();
        tabScene->setLayout(std::make_shared<BorderLayout>());
        tabScene->setFlexible(true);
        {
            auto vbox = Box::createVerticalBox();

            auto description1 = std::make_shared<Label>();
            description1->setFont(font);
            description1->setText(u"ボタンを押下するとそれぞれのシーンへ遷移します。");
            description1->setPreferredSize(font->measure(description1->getFontSize(), description1->getText()));
            vbox->addLayoutElement(std::make_shared<LayoutElement>(description1, nullptr));

            auto buttonDemo2D = std::make_shared<Button>();
            buttonDemo2D->setFont(font);
            buttonDemo2D->setText(u"Demo(2D)");
            buttonDemo2D->setPreferredSize(Vector2({ 600, 80 }));
            buttonDemo2D->setOnClick([&]() -> void { this->m_nextScene = "Demo2D"; });
            vbox->addLayoutElement(std::make_shared<LayoutElement>(buttonDemo2D, nullptr));

            auto buttonDemo3D = std::make_shared<Button>();
            buttonDemo3D->setFont(font);
            buttonDemo3D->setText(u"Demo(3D)");
            buttonDemo3D->setPreferredSize(Vector2({ 600, 80 }));
            buttonDemo3D->setOnClick([&]() -> void { this->m_nextScene = "Demo3D"; });
            vbox->addLayoutElement(std::make_shared<LayoutElement>(buttonDemo3D, nullptr));

            auto buttonDemoUI = std::make_shared<Button>();
            buttonDemoUI->setFont(font);
            buttonDemoUI->setText(u"Demo(UI)");
            buttonDemoUI->setPreferredSize(Vector2({ 600, 80 }));
            buttonDemoUI->setOnClick([&]() -> void { this->m_nextScene = "DemoUI"; });
            vbox->addLayoutElement(std::make_shared<LayoutElement>(buttonDemoUI, nullptr));

            auto buttonGame = std::make_shared<Button>();
            buttonGame->setFont(font);
            buttonGame->setText(u"GAME");
            buttonGame->setPreferredSize(Vector2({ 600, 80 }));
            buttonGame->setOnClick([&]() -> void { this->m_nextScene = "Title"; });
            vbox->addLayoutElement(std::make_shared<LayoutElement>(buttonGame, nullptr));

            tabScene->addLayoutElement(std::make_shared<LayoutElement>(vbox, std::make_shared<BorderLayout::Hint>("CENTER")));
        }

        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tabHome, nullptr));
        tabbedPane->setTitleAt(0, u"HOME");

        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tabScene, nullptr));
        tabbedPane->setTitleAt(1, u"SCENE");

        m_rootPane->addLayoutElement(std::make_shared<LayoutElement>(tabbedPane, std::make_shared<BorderLayout::Hint>("CENTER")));
        m_rootPane->doLayout();
    }
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    m_nextScene = "";
}

void LauncherScene::onExit()
{
}

void LauncherScene::onUpdate()
{
    m_rootPane->update();
}

void LauncherScene::onGui()
{
}

void LauncherScene::onDraw3D()
{
}

void LauncherScene::onDraw2D()
{
    m_rootPane->draw2D(m_renderer);
}

bool LauncherScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
}