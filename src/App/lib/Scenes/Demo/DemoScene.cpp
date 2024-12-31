#include <Common/Graphics/Node.hpp>
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
    , m_sceneCompleted()
    , m_particleVertexBuffer()
    , m_particleIndexBuffer()
    , m_particleInstanceSourceBuffer()
    , m_particleInstanceBuffer()
    , m_particleDualBuffer(nullptr, nullptr)
    , m_particleIndexLength()
    , m_renderer()
{
}
DemoScene::~DemoScene() { }

void DemoScene::onEnter()
{
    auto tex = Texture::create("./assets/tileNormal2.png");
    if (!m_particleVertexBuffer || !m_particleIndexBuffer) {
        std::vector<VertexNormal3D> vertices;
        std::vector<uint32_t> indices;
        Polygon::generateBox(vertices, indices);

        m_particleVertexBuffer = Buffer::create(Buffer::Type::Vertex);
        m_particleVertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
        m_particleVertexBuffer->update(vertices.data());

        m_particleIndexBuffer = Buffer::create(Buffer::Type::Vertex);
        m_particleIndexLength = static_cast<int32_t>(indices.size());
        m_particleIndexBuffer->allocate(sizeof(uint32_t) * indices.size());
        m_particleIndexBuffer->update(indices.data());

        m_particleInstanceSourceBuffer = Buffer::create(Buffer::Type::Vertex);
        std::vector<VertexParticle3D> particles(3000);
        Random random;
        for (int32_t i = 0; i < 3000; i++) {
            VertexParticle3D& particle = particles.at(i);
            float fx = random.range(0.0f, 1.0f);
            float fy = random.range(0.0f, 1.0f);
            float fz = random.range(0.0f, 1.0f);
            particle.velocity = Vector3::normalized(Vector3({ fx, fy, fz }));
            particle.offset = Vector3({ 0, 0, 0 });
        }
        m_particleInstanceSourceBuffer->allocate(sizeof(VertexParticle3D) * particles.size());
        m_particleInstanceSourceBuffer->update(particles.data());

        m_particleInstanceBuffer = Buffer::create(Buffer::Type::ReadWrite);
        m_particleInstanceBuffer->allocate(m_particleInstanceSourceBuffer->getSize());

        m_particleDualBuffer.reset(m_particleInstanceSourceBuffer, m_particleInstanceBuffer);
    }
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    m_sceneCompleted = false;
    m_globalLightDir = Vector3({ 1, 1, 0 });
    m_pointLightPositions.clear();
    m_pointLightPositions.emplace_back(Vector3({ 8, 0, 8 }));
}
void DemoScene::onExit()
{
}

void DemoScene::onUpdate()
{
    m_fpsController.update();
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

    auto ub = UniformPool::rent(Metadata::ProgramTable::ParticleInstance3D);
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = Matrix();
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = Vector4({ 1, 1, 1, 1 });
    ub->setVS(1, &uColor);

    Reflect::UFloat uDeltatime;
    uDeltatime.value = Time::deltaTime();
    ub->setCS(0, &uDeltatime);
    ub->setCS(1, m_particleInstanceBuffer);

    auto rc = RenderContext::get(Metadata::ProgramTable::ParticleInstance3D);

    auto surface = Engine::getInstance()->getDevice()->getSurface();
    surface->sync(m_particleDualBuffer);
    surface->compute(rc, ub, 3000 / 256, 1, 1);
    surface->render(rc, ub, m_particleVertexBuffer, m_particleIndexBuffer, m_particleIndexLength, std::vector<std::shared_ptr<Buffer>> { m_particleInstanceBuffer }, 3000);
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

    m_renderer->textFont(FontFactory::getInstance()->load("./assets/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(backgroundCenter - Vector2({ 0, 100 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"2D描画のテスト", Vector4({ 0, 0, 0, 1 }));
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