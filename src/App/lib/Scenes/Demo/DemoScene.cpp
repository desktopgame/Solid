#include <Common/Graphics/Node.hpp>
#include <Common/Graphics/ParticleSystem.hpp>
#include <Common/Graphics/SphericalParticle.hpp>
#include <Scenes/Demo/DemoScene.hpp>
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
        m_root = std::make_shared<TabbedPane>();
        m_root->setFont(font);
        m_root->setPosition(Vector2({ 200, 150 }));
        m_root->setSize(Vector2({ 300, 300 }));

        // tab1 -- BorderLayoutのテスト
        auto tab1 = std::make_shared<Panel>();
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
        m_root->addLayoutElement(std::make_shared<LayoutElement>(tab1, nullptr));
        m_root->setTitleAt(0, u"BorderLayout");

        // tab2 -- BorderLayoutのテスト
        auto tab2 = std::make_shared<Panel>();
        tab2->setLayout(std::make_shared<BorderLayout>());
        tab2->setFlexible(true);
        auto vbox = Box::createVerticalBox();
        auto hbox1 = Box::createHorizontalBox();

        auto button1 = std::make_shared<Button>();
        button1->setFont(font);
        button1->setText(u"Button1");
        button1->setPreferredSize(Vector2({ 80, 40 }));
        hbox1->addLayoutElement(std::make_shared<LayoutElement>(button1, nullptr));

        auto button2 = std::make_shared<Button>();
        button2->setFont(font);
        button2->setText(u"Button2");
        button2->setPreferredSize(Vector2({ 80, 40 }));
        hbox1->addLayoutElement(std::make_shared<LayoutElement>(button2, nullptr));

        auto hbox2 = Box::createHorizontalBox();

        auto button3 = std::make_shared<Button>();
        button3->setFont(font);
        button3->setText(u"Button3");
        button3->setPreferredSize(Vector2({ 80, 40 }));
        hbox2->addLayoutElement(std::make_shared<LayoutElement>(button3, nullptr));

        auto button4 = std::make_shared<Button>();
        button4->setFont(font);
        button4->setText(u"Button4");
        button4->setPreferredSize(Vector2({ 80, 40 }));
        hbox2->addLayoutElement(std::make_shared<LayoutElement>(button4, nullptr));

        vbox->addLayoutElement(std::make_shared<LayoutElement>(hbox1, nullptr));
        vbox->addLayoutElement(std::make_shared<LayoutElement>(hbox2, nullptr));
        tab2->addLayoutElement(std::make_shared<LayoutElement>(vbox, std::make_shared<BorderLayout::Hint>("CENTER")));
        m_root->addLayoutElement(std::make_shared<LayoutElement>(tab2, nullptr));
        m_root->setTitleAt(1, u"BoxLayout");

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
        PointLight::set(i, m_pointLightPositions.at(i), 1, 5);
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
    Vector2 backgroundCenter = Vector2({ -Screen::getWidth() / 4.0f, Screen::getHeight() / 4.0f });
    Vector2 backgroundSize = (Vector2)Screen::getSize() / 2.0f;
    m_renderer->drawRect(
        backgroundCenter,
        backgroundSize,
        0.0f,
        Color({ 1.0f, 1.0f, 1.0f, 1.0f }));

    m_renderer->drawCircle(backgroundCenter + Vector2({ -50, 0 }), Vector2({ 50, 50 }), Vector4({ 1, 0, 0, 1 }));
    m_renderer->drawRect(backgroundCenter + Vector2({ 50, 0 }), Vector2({ 50, 50 }), 45.0f, Vector4({ 0, 1, 0, 1 }));

    m_renderer->textFont(FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(backgroundCenter - Vector2({ 0, 100 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"2D描画のテスト", Vector4({ 0, 0, 0, 1 }));

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