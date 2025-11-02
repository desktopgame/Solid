#include <App/Scenes/DemoUI/DemoUIScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::DemoUI {
// public
DemoUIScene::DemoUIScene()
    : m_nextScene()
    , m_renderer()
{
}
DemoUIScene::~DemoUIScene() { }

void DemoUIScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_backButton) {
        m_backButton = std::make_unique<App::Common::Util::BackButton>();
        m_backButton->init(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"), [&]() -> void {
            m_nextScene = "Launcher";
        });
    }
    if (!m_root) {
        auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
        m_root = RootPane::create();
        m_root->setPosition(Vector2({ 0, 0 }));
        m_root->setSize(Vector2({ 800, 600 }));

        auto content = std::make_shared<Panel>();
        content->setFlexible(true);
        content->setLayout(std::make_shared<BorderLayout>());

        auto header = Box::createHorizontalBox();
        auto title = std::make_shared<Label>();
        title->setFont(font);
        title->setText(u"UIのテスト");
        title->setPreferredSize(font->measure(title->getFontSize(), title->getText()));
        header->addLayoutElement(std::make_shared<LayoutElement>(title, nullptr));
        content->addLayoutElement(std::make_shared<LayoutElement>(header, std::make_shared<BorderLayout::Hint>("TOP")));

        auto tabbedPane = std::make_shared<TabbedPane>();
        tabbedPane->setFont(font);
        content->addLayoutElement(std::make_shared<LayoutElement>(tabbedPane, std::make_shared<BorderLayout::Hint>("CENTER")));

        m_root->addLayoutElement(std::make_shared<LayoutElement>(content, std::make_shared<BorderLayout::Hint>("CENTER")));

        // tab1 -- BorderLayoutのテスト
        auto tab1
            = std::make_shared<Panel>();
        tab1->setLayout(std::make_shared<BorderLayout>());
        tab1->setFlexible(true);
        auto topCI = std::make_shared<ColorIcon>();
        topCI->setForegroundColor(Color({ 1, 0, 0, 1 }));
        topCI->setPreferredSize(Vector2({ 50, 50 }));
        topCI->setFlexible(true);
        tab1->addLayoutElement(std::make_shared<LayoutElement>(topCI, std::make_shared<BorderLayout::Hint>("TOP")));
        auto bottomCI = std::make_shared<ColorIcon>();
        bottomCI->setForegroundColor(Color({ 0, 1, 0, 1 }));
        bottomCI->setPreferredSize(Vector2({ 50, 50 }));
        bottomCI->setFlexible(true);
        tab1->addLayoutElement(std::make_shared<LayoutElement>(bottomCI, std::make_shared<BorderLayout::Hint>("BOTTOM")));
        auto leftCI = std::make_shared<ColorIcon>();
        leftCI->setForegroundColor(Color({ 0, 0, 1, 1 }));
        leftCI->setPreferredSize(Vector2({ 50, 50 }));
        leftCI->setFlexible(true);
        tab1->addLayoutElement(std::make_shared<LayoutElement>(leftCI, std::make_shared<BorderLayout::Hint>("LEFT")));
        auto rightCI = std::make_shared<ColorIcon>();
        rightCI->setForegroundColor(Color({ 0, 1, 1, 1 }));
        rightCI->setPreferredSize(Vector2({ 50, 50 }));
        rightCI->setFlexible(true);
        tab1->addLayoutElement(std::make_shared<LayoutElement>(rightCI, std::make_shared<BorderLayout::Hint>("RIGHT")));
        auto centerCI = std::make_shared<ColorIcon>();
        centerCI->setForegroundColor(Color({ 1, 1, 1, 1 }));
        centerCI->setPreferredSize(Vector2({ 50, 50 }));
        centerCI->setFlexible(true);
        tab1->addLayoutElement(std::make_shared<LayoutElement>(centerCI, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tab1, nullptr));
        tabbedPane->setTitleAt(0, u"BorderLayout");

        // tab2 -- BoxLayoutのテスト
        auto tab2 = std::make_shared<Panel>();
        tab2->setLayout(std::make_shared<BorderLayout>());
        tab2->setFlexible(true);
        auto vbox = Box::createVerticalBox();
        auto hbox1 = Box::createHorizontalBox();
        hbox1->setBackgroundColor(Color({ 0.1f, 0.5f, 0.1f, 1.0f }));

        auto label1 = std::make_shared<Label>();
        label1->setFont(font);
        label1->setText(u"Label");
        label1->setPreferredSize(Vector2({ 80, 40 }));
        hbox1->addLayoutElement(std::make_shared<LayoutElement>(label1, nullptr));

        auto button2 = std::make_shared<Button>();
        button2->setFont(font);
        button2->setText(u"Button2");
        button2->setPreferredSize(Vector2({ 80, 40 }));
        hbox1->addLayoutElement(std::make_shared<LayoutElement>(button2, nullptr));

        auto hbox2 = Box::createHorizontalBox();
        hbox2->setBackgroundColor(Color({ 0.5f, 0.1f, 0.1f, 1.0f }));

        auto slider1 = std::make_shared<Slider>();
        slider1->setPreferredSize(Vector2({ 80, 40 }));
        slider1->setValue(0.0f);
        hbox2->addLayoutElement(std::make_shared<LayoutElement>(slider1, nullptr));

        auto toggle = std::make_shared<Toggle>();
        toggle->setPreferredSize(Vector2({ 80, 40 }));
        hbox2->addLayoutElement(std::make_shared<LayoutElement>(toggle, nullptr));

        vbox->addLayoutElement(std::make_shared<LayoutElement>(hbox1, nullptr));
        vbox->addLayoutElement(std::make_shared<LayoutElement>(hbox2, nullptr));
        tab2->addLayoutElement(std::make_shared<LayoutElement>(vbox, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tab2, nullptr));
        tabbedPane->setTitleAt(1, u"BoxLayout");

        // tab3 -- ScrollPaneのテスト
        auto tab3 = std::make_shared<Panel>();
        tab3->setLayout(std::make_shared<BorderLayout>());
        tab3->setFlexible(true);
        auto scrollContent = Box::createVerticalBox();
        scrollContent->setFlexible(true);
        scrollContent->setBackgroundColor(Color({ 1, 0, 0, 1 }));
        for (int32_t i = 0; i < 10; i++) {
            auto line = Box::createHorizontalBox();
            line->setFlexible(true);
            for (int32_t j = 0; j < 10; j++) {
                auto sample = std::make_shared<Button>();
                sample->setFont(font);
                sample->setText(u"Sample");
                sample->setPreferredSize(Vector2({ 80, 40 }));
                line->addLayoutElement(std::make_shared<LayoutElement>(sample, nullptr));
            }
            scrollContent->addLayoutElement(std::make_shared<LayoutElement>(line, nullptr));
        }
        auto scrollPane = std::make_shared<ScrollPane>();
        scrollPane->setFlexible(true);
        scrollPane->setView(scrollContent);
        tab3->addLayoutElement(std::make_shared<LayoutElement>(scrollPane, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tab3, nullptr));
        tabbedPane->setTitleAt(2, u"ScrollPane");

        // tab4 -- Formのテスト
        auto tab4 = std::make_shared<Panel>();
        tab4->setLayout(std::make_shared<BorderLayout>());
        tab4->setFlexible(true);
        auto form = std::make_shared<Form>(font, 16, Vector2({ 120, 50 }));
        form->addLabel(u"Label", u"TextValue");
        form->addButton(u"Button", u"ClickMe", nullptr);
        form->addToggle(u"Toggle", nullptr);
        form->addSlider(u"Slider", nullptr);

        tab4->addLayoutElement(std::make_shared<LayoutElement>(form, std::make_shared<BorderLayout::Hint>("CENTER")));
        tabbedPane->addLayoutElement(std::make_shared<LayoutElement>(tab4, nullptr));
        tabbedPane->setTitleAt(3, u"Form");

        m_root->doLayout();
    }
    m_nextScene = "";
}

void DemoUIScene::onExit()
{
}

void DemoUIScene::onUpdate()
{
    m_root->update();
    m_backButton->update();
}

void DemoUIScene::onGui()
{
    ImGui::Begin("DemoUI");
    ImGui::SeparatorText("Scene Transition");
    if (ImGui::Button("Exit")) {
        m_nextScene = "Launcher";
    }
    ImGui::End();
}

void DemoUIScene::onDraw3D()
{
}

void DemoUIScene::onDraw2D()
{
    m_backButton->draw(m_renderer);
    m_root->draw2D(m_renderer);
}

bool DemoUIScene::tryTransition(std::string& outNextScene)
{
    if (m_nextScene != "") {
        outNextScene = m_nextScene;
        return true;
    }
    return false;
}
// private
}