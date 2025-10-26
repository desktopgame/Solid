#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Graphics/ParticleSystem.hpp>
#include <App/Common/Graphics/SphericalParticle.hpp>
#include <App/Scenes/Demo/DemoScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Demo {
// public
DemoScene::DemoScene()
    : m_fpsController()
    , m_isDraw2D(true)
    , m_isDraw3D(true)
    , m_globalLightDir({ 1, 1, 0 })
    , m_pointLightPos({ 0, 0, 0 })
    , m_pointLightPositions()
    , m_root()
    , m_sceneCompleted()
    , m_renderer()
{
}
DemoScene::~DemoScene() { }

void DemoScene::onEnter()
{
    auto tex = Texture::create("./assets/tileNormal2.png");
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    m_sceneCompleted = false;
    m_globalLightDir = Vector3({ 1, 1, 0 });
    m_pointLightPositions.clear();
    m_pointLightPositions.emplace_back(Vector3({ 8, 0, 8 }));
    if (!m_root) {
        auto font = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
        m_root = RootPane::create();
        m_root->setPosition(Vector2({ 0, 0 }));
        m_root->setSize(Vector2({ 500, 400 }));

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
}
void DemoScene::onExit()
{
}

void DemoScene::onUpdate()
{
    m_fpsController.update();
    m_root->update();
}

void DemoScene::onGui()
{
    ImGui::Begin("Demo");
    ImGui::SeparatorText("General");
    ImGui::Checkbox("Draw2D", &m_isDraw2D);
    ImGui::Checkbox("Draw3D", &m_isDraw3D);
    ImGui::SeparatorText("Light");
    ImGui::DragFloat3("GlobalLightDir", m_globalLightDir.data(), 0.01f);
    for (int32_t i = 0; i < m_pointLightPositions.size(); i++) {
        char buf[32];
        ::sprintf(buf, "PointLight[%d]", i);
        ImGui::DragFloat3(buf, m_pointLightPositions.at(i).data(), 0.01f);
    }
    ImGui::DragFloat3("Next PointLight", m_pointLightPos.data(), 0.01f);
    if (ImGui::Button("Add PointLight")) {
        m_pointLightPositions.push_back(m_pointLightPos);
    }
    if (ImGui::Button("Show Particle")) {

        Common::Graphics::ParticleSystem::emit<Common::Graphics::SphericalParticle>(Common::Graphics::ParticleParameter<Common::Graphics::SphericalParticleOption>(
            Vector3({ 1.0f, 0.0f, 0.0f }),
            Vector3({ 0.01f, 0.01f, 0.01f }),
            0.25f,
            1.0f,
            Common::Graphics::SphericalParticleOption(Vector3({ 0.0f, 0.0f, 0.0f }), 1.0f, 20.0f)));
    }
    ImGui::SeparatorText("Scene Transition");
    if (ImGui::Button("Exit")) {
        m_sceneCompleted = true;
    }
    ImGui::End();
}
void DemoScene::onDraw3D()
{
    if (!m_isDraw3D) {
        return;
    }

    Camera::position(m_fpsController.getPosition());
    Camera::lookAt(m_fpsController.getLookAt());

    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(m_globalLightDir));

    PointLight::enable();
    for (int32_t i = 0; i < m_pointLightPositions.size(); i++) {
        PointLight::set(i, m_pointLightPositions.at(i), 1, 5, Vector3({ 1, 1, 1 }));
    }
    PointLight::setCount(m_pointLightPositions.size());

    Quaternion q;
    m_renderer->drawPlane(Vector3({ 10, 0, 10 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawPlane(Vector3({ 10, 0, 20 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), true);
    m_renderer->drawBox(Vector3({ 25, 0, 10 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawBox(Vector3({ 25, 0, 20 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), true);
    Common::Graphics::ParticleSystem::draw();
}

void DemoScene::onDraw2D()
{
    if (!m_isDraw2D) {
        return;
    }
    Vector2 backgroundCenter = Vector2({ -325, 50 });
    Vector2 backgroundSize = Vector2({ 100, 200 });
    m_renderer->drawRect(
        backgroundCenter,
        backgroundSize,
        0.0f,
        Color({ 1.0f, 1.0f, 1.0f, 1.0f }));

    m_renderer->drawCircle(backgroundCenter + Vector2({ 0, -30 }), Vector2({ 50, 50 }), Vector4({ 1, 0, 0, 1 }));
    m_renderer->drawRect(backgroundCenter + Vector2({ 0, 50 }), Vector2({ 50, 50 }), 45.0f, Vector4({ 0, 1, 0, 1 }));

    m_renderer->textFont(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(backgroundCenter - Vector2({ 0, 80 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"テスト", Vector4({ 0, 0, 0, 1 }));

    m_root->draw2D(m_renderer);
}

bool DemoScene::tryTransition(std::string& outNextScene)
{
    if (m_sceneCompleted) {
        outNextScene = "Debug";
    }
    return m_sceneCompleted;
}
// private
}