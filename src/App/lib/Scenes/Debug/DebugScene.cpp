#include <Common/Constants.hpp>
#include <Scenes/Debug/DebugScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Debug {
// public
DebugScene::DebugScene()
    : m_cameraPos({ 0, 0, -3 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.08f)
    , m_cameraRotateSpeed(0.8f)
    , m_cursorVisible(true)
    , m_editCompleted()
    , m_renderer()
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_tileTransform()
    , m_tilePallet()
    , m_instanceBuffers()
{
}
DebugScene::~DebugScene() { }

void DebugScene::onEnter()
{
    auto tex = Texture::create("./assets/tileNormal2.png");
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_debugTexture) {
        m_debugTexture = Texture::create("./assets/tileNormal2.png");
    }
    if (!m_vertexBuffer) {
        m_vertexBuffer = Buffer::create();
        m_indexBuffer = Buffer::create();

        std::vector<VertexNormalTexCoord3D> vertices;
        std::vector<uint32_t> indices;
        Polygon::generatePlane(vertices, indices);
        m_indexLength = static_cast<int32_t>(indices.size());

        m_vertexBuffer->allocate(sizeof(VertexNormalTexCoord3D) * vertices.size());
        m_vertexBuffer->update(vertices.data());

        m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
        m_indexBuffer->update(indices.data());

        auto instBuf = Buffer::create();
        std::vector<Vector4> instances;
        for (int32_t i = 0; i < 10; i++) {
            instances.emplace_back(Vector4({ 0, 0, static_cast<float>(i), static_cast<float>((10 * i) + 0) }));
            instances.emplace_back(Vector4({ 0, 0, static_cast<float>(i), static_cast<float>((10 * i) + 1) }));
            instances.emplace_back(Vector4({ 0, 0, static_cast<float>(i), static_cast<float>((10 * i) + 2) }));
            instances.emplace_back(Vector4({ 0, 0, static_cast<float>(i), static_cast<float>((10 * i) + 3) }));
            instances.emplace_back(Vector4({ 0, 0, static_cast<float>(i), static_cast<float>((10 * i) + 4) }));
            instances.emplace_back(Vector4({ 0, 0, static_cast<float>(i), static_cast<float>((10 * i) + 5) }));
        }
        instBuf->allocate(sizeof(Vector4) * instances.size());
        instBuf->update(instances.data());
        m_instanceBuffers.emplace_back(instBuf);

        const float tileSize = 1.0f;
        const float k_tileHalf = tileSize / 2.0f;
        m_tileTransform.translateMatrixTable = {
            // posY
            Matrix::translate(Vector3({ 0.0f, k_tileHalf, 0.0f })),
            // negY
            Matrix::translate(Vector3({ 0.0f, -k_tileHalf, 0.0f })),
            // posX
            Matrix::translate(Vector3({ k_tileHalf, 0.0f, 0.0f })),
            // negX
            Matrix::translate(Vector3({ -k_tileHalf, 0.0f, 0.0f })),
            // posZ
            Matrix::translate(Vector3({ 0.0f, 0.0f, k_tileHalf })),
            // negZ
            Matrix::translate(Vector3({ 0.0f, 0.0f, -k_tileHalf })),
        };
        m_tileTransform.rotationMatrixTable = {
            // posY
            Matrix::rotateX(-90.0f),
            // negY
            Matrix::rotateX(90.0f),
            // posX
            Matrix::rotateY(90.0f),
            // negX
            Matrix::rotateY(-90.0f),
            // posZ
            Matrix::rotateY(180.0f),
            // negZ
            Matrix(),
        };
        m_tileTransform.normalVectorTable = {
            // posY
            Vector3({ 0, 1, 0 }),
            // negY
            Vector3({ 0, -1, 0 }),
            // posX
            Vector3({ 1, 0, 0 }),
            // negX
            Vector3({ -1, 0, 0 }),
            // posZ
            Vector3({ 0, 0, 1 }),
            // negZ
            Vector3({ 0, 0, -1 }),
        };
        m_tilePallet.table = {
            // https://lospec.com/palette-list/tag/64
            // Jehkoba64
            Vector4({ 0.0f, 0.0f, 0.0f, 1.0f }),
            Vector4({ 0.9803921568627451f, 0.7333333333333333f, 0.6862745098039216f, 1.0f }),
            Vector4({ 0.9215686274509803f, 0.4588235294117647f, 0.5607843137254902f, 1.0f }),
            Vector4({ 0.8509803921568627f, 0.2980392156862745f, 0.5568627450980392f, 1.0f }),
            Vector4({ 0.7019607843137254f, 0.17647058823529413f, 0.49019607843137253f, 1.0f }),
            Vector4({ 0.9803921568627451f, 0.596078431372549f, 0.5686274509803921f, 1.0f }),
            Vector4({ 1.0f, 0.4392156862745098f, 0.4392156862745098f, 1.0f }),
            Vector4({ 0.9607843137254902f, 0.19215686274509805f, 0.2549019607843137f, 1.0f }),
            Vector4({ 0.7686274509803922f, 0.047058823529411764f, 0.1803921568627451f, 1.0f }),
            Vector4({ 0.5215686274509804f, 0.13333333333333333f, 0.39215686274509803f, 1.0f }),
            Vector4({ 0.9803921568627451f, 0.6274509803921569f, 0.19607843137254902f, 1.0f }),
            Vector4({ 0.9607843137254902f, 0.5058823529411764f, 0.13333333333333333f, 1.0f }),
            Vector4({ 0.9490196078431372f, 0.3843137254901961f, 0.12156862745098039f, 1.0f }),
            Vector4({ 0.8588235294117647f, 0.29411764705882354f, 0.08627450980392157f, 1.0f }),
            Vector4({ 0.6196078431372549f, 0.2980392156862745f, 0.2980392156862745f, 1.0f }),
            Vector4({ 0.9803921568627451f, 0.8509803921568627f, 0.21568627450980393f, 1.0f }),
            Vector4({ 1.0f, 0.7254901960784313f, 0.2196078431372549f, 1.0f }),
            Vector4({ 0.9019607843137255f, 0.6078431372549019f, 0.13333333333333333f, 1.0f }),
            Vector4({ 0.8f, 0.5019607843137255f, 0.1607843137254902f, 1.0f }),
            Vector4({ 0.6784313725490196f, 0.41568627450980394f, 0.27058823529411763f, 1.0f }),
            Vector4({ 0.8f, 0.7803921568627451f, 0.23921568627450981f, 1.0f }),
            Vector4({ 0.7019607843137254f, 0.6901960784313725f, 0.17647058823529413f, 1.0f }),
            Vector4({ 0.596078431372549f, 0.611764705882353f, 0.15294117647058825f, 1.0f }),
            Vector4({ 0.5490196078431373f, 0.5019607843137255f, 0.1411764705882353f, 1.0f }),
            Vector4({ 0.47843137254901963f, 0.3686274509803922f, 0.21568627450980393f, 1.0f }),
            Vector4({ 0.5803921568627451f, 0.7490196078431373f, 0.18823529411764706f, 1.0f }),
            Vector4({ 0.3333333333333333f, 0.7019607843137254f, 0.23137254901960785f, 1.0f }),
            Vector4({ 0.09019607843137255f, 0.611764705882353f, 0.2627450980392157f, 1.0f }),
            Vector4({ 0.023529411764705882f, 0.5019607843137255f, 0.3176470588235294f, 1.0f }),
            Vector4({ 0.06666666666666667f, 0.3764705882352941f, 0.3803921568627451f, 1.0f }),
            Vector4({ 0.6274509803921569f, 0.9215686274509803f, 0.6588235294117647f, 1.0f }),
            Vector4({ 0.48627450980392156f, 0.8117647058823529f, 0.6039215686274509f, 1.0f }),
            Vector4({ 0.3607843137254902f, 0.7215686274509804f, 0.5333333333333333f, 1.0f }),
            Vector4({ 0.23921568627450981f, 0.6313725490196078f, 0.49411764705882355f, 1.0f }),
            Vector4({ 0.12549019607843137f, 0.5019607843137255f, 0.4235294117647059f, 1.0f }),
            Vector4({ 0.28627450980392155f, 0.7607843137254902f, 0.9490196078431372f, 1.0f }),
            Vector4({ 0.1450980392156863f, 0.6745098039215687f, 0.9607843137254902f, 1.0f }),
            Vector4({ 0.09019607843137255f, 0.5764705882352941f, 0.9019607843137255f, 1.0f }),
            Vector4({ 0.10980392156862745f, 0.4588235294117647f, 0.7411764705882353f, 1.0f }),
            Vector4({ 0.09803921568627451f, 0.3568627450980392f, 0.6509803921568628f, 1.0f }),
            Vector4({ 0.6823529411764706f, 0.5333333333333333f, 0.8901960784313725f, 1.0f }),
            Vector4({ 0.49411764705882355f, 0.49411764705882355f, 0.9490196078431372f, 1.0f }),
            Vector4({ 0.34509803921568627f, 0.41568627450980394f, 0.7686274509803922f, 1.0f }),
            Vector4({ 0.20784313725490197f, 0.3254901960784314f, 0.6509803921568628f, 1.0f }),
            Vector4({ 0.1411764705882353f, 0.2235294117647059f, 0.4f, 1.0f }),
            Vector4({ 0.8862745098039215f, 0.6078431372549019f, 0.9803921568627451f, 1.0f }),
            Vector4({ 0.792156862745098f, 0.49411764705882355f, 0.9490196078431372f, 1.0f }),
            Vector4({ 0.6392156862745098f, 0.36470588235294116f, 0.8509803921568627f, 1.0f }),
            Vector4({ 0.4666666666666667f, 0.23137254901960785f, 0.7490196078431373f, 1.0f }),
            Vector4({ 0.3058823529411765f, 0.15294117647058825f, 0.5490196078431373f, 1.0f }),
            Vector4({ 0.7098039215686275f, 0.5490196078431373f, 0.4980392156862745f, 1.0f }),
            Vector4({ 0.6196078431372549f, 0.4666666666666667f, 0.403921568627451f, 1.0f }),
            Vector4({ 0.5294117647058824f, 0.36470588235294116f, 0.34509803921568627f, 1.0f }),
            Vector4({ 0.43137254901960786f, 0.25882352941176473f, 0.3137254901960784f, 1.0f }),
            Vector4({ 0.2784313725490196f, 0.1803921568627451f, 0.24313725490196078f, 1.0f }),
            Vector4({ 0.6509803921568628f, 0.6039215686274509f, 0.611764705882353f, 1.0f }),
            Vector4({ 0.5019607843137255f, 0.4745098039215686f, 0.5019607843137255f, 1.0f }),
            Vector4({ 0.4117647058823529f, 0.396078431372549f, 0.4392156862745098f, 1.0f }),
            Vector4({ 0.28627450980392155f, 0.3176470588235294f, 0.4117647058823529f, 1.0f }),
            Vector4({ 0.050980392156862744f, 0.12941176470588237f, 0.25098039215686274f, 1.0f }),
            Vector4({ 0.0196078431372549f, 0.054901960784313725f, 0.10196078431372549f, 1.0f }),
            Vector4({ 0.8509803921568627f, 0.6549019607843137f, 0.596078431372549f, 1.0f }),
            Vector4({ 0.7686274509803922f, 0.7333333333333333f, 0.7019607843137254f, 1.0f }),
            Vector4({ 0.9490196078431372f, 0.9490196078431372f, 0.8549019607843137f, 1.0f }),
        };
    }
    m_editCompleted = false;
}
void DebugScene::onExit()
{
}

void DebugScene::onUpdate()
{
    (void)m_cameraAngleX;
    (void)m_cameraAngleY;

    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();
    if (Cursor::isLocked()) {
        if (keyboard->isPressed(KeyCode::W)) {
            m_cameraPos += forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::S)) {
            m_cameraPos -= forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::D)) {
            m_cameraPos += right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::A)) {
            m_cameraPos -= right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        }
        if (keyboard->isPressed(KeyCode::Space)) {
            m_cameraPos += Vector3({ 0, 1, 0 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::Shift)) {
            m_cameraPos -= Vector3({ 0, 1, 0 }) * m_cameraMoveSpeed;
        }
        m_cameraLookAt = m_cameraPos + forward;

        auto mouseDelta = mouse->getDelta();
        m_cameraAngleX -= static_cast<float>(mouseDelta.y()) * m_cameraRotateSpeed;
        m_cameraAngleY += static_cast<float>(mouseDelta.x()) * m_cameraRotateSpeed;

        if (m_cameraAngleX <= -90.0f) {
            m_cameraAngleX = -89.0f;
        }
        if (m_cameraAngleX >= 90.0f) {
            m_cameraAngleX = 89.0f;
        }
    }

    if (keyboard->isTrigger(KeyCode::E)) {
        if (m_cursorVisible) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();
        }
        m_cursorVisible = !m_cursorVisible;
    }
}

void DebugScene::onGui()
{
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Pos", m_cameraPos.data());
    ImGui::DragFloat3("LookAt", m_cameraLookAt.data());
    ImGui::DragFloat("AngleX", &m_cameraAngleX);
    ImGui::DragFloat("AngleY", &m_cameraAngleY);
    ImGui::DragFloat("MoveSpeed", &m_cameraMoveSpeed, 0.01f);
    ImGui::DragFloat("RotationSpeed", &m_cameraRotateSpeed, 0.01f);
    ImGui::End();

    ImGui::Begin("Cursor");
    auto pos = InputSystem::getInstance()->getMouse()->getPosition();
    auto delta = InputSystem::getInstance()->getMouse()->getDelta();
    ImGui::LabelText("Pos", "%d, %d", pos.x(), pos.y());
    ImGui::LabelText("Delta", "%d, %d", delta.x(), delta.y());
    ImGui::End();

    ImGui::Begin("Menu");
    if (ImGui::Button("Exit")) {
        m_editCompleted = true;
    }
    ImGui::End();
}
void DebugScene::onDraw3D()
{
    Camera::position(m_cameraPos);
    Camera::lookAt(m_cameraLookAt);

    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(Vector3({ 1, 1, 0 })));

    PointLight::enable();
    PointLight::set(0, Vector3({ 8, 0, 6 }), 1, 5);
    PointLight::set(1, Vector3({ -8, 0, 6 }), 1, 5);
    PointLight::set(2, Vector3({ 20, 0, 6 }), 1, 5);
    PointLight::set(3, Vector3({ -20, 0, 6 }), 1, 5);
    PointLight::setCount(4);

    Quaternion q;
    m_renderer->drawPlane(Vector3({ 10, 0, 10 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawPlane(Vector3({ 10, 0, 20 }), Vector2({ 10, 10 }), q, Vector4({ 1, 1, 1, 1 }), true);
    m_renderer->drawPlaneTexture(Vector3({ -10, 0, 10 }), Vector2({ 10, 10 }), q, m_debugTexture, Vector4({ 1, 1, 1, 1 }));
    m_renderer->drawBox(Vector3({ 25, 0, 10 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), false);
    m_renderer->drawBox(Vector3({ 25, 0, 20 }), Vector3({ 5, 5, 5 }), q, Vector4({ 1, 1, 1, 1 }), true);
    m_renderer->drawBoxTexture(Vector3({ -25, 0, 10 }), Vector3({ 5, 5, 5 }), q, m_debugTexture, Vector4({ 1, 1, 1, 1 }));

    auto rc = RenderContext::get(Metadata::ProgramTable::Tile3D);
    auto ub = UniformPool::rent(Metadata::ProgramTable::Tile3D);
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = Matrix::transform(
        Matrix::translate(Vector3({ 0, 0, 0 })),
        Matrix(),
        Matrix::scale(Vector3({ 1, 1, 1 })));
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);
    ub->setVS(1, &m_tileTransform);
    ub->setVS(2, &m_tilePallet);

    ub->setPS(0, m_debugTexture);
    Engine::getInstance()->getDevice()->getSurface()->render(
        rc,
        ub,
        m_vertexBuffer,
        m_indexBuffer,
        m_indexLength,
        m_instanceBuffers,
        60);
}

void DebugScene::onDraw2D()
{
    m_renderer->drawCircle(Vector2({ 0, 0 }), Vector2({ 50, 50 }), Vector4({ 1, 1, 1, 1 }));
    m_renderer->drawRect(Vector2({ 100, 100 }), Vector2({ 50, 50 }), 45.0f, Vector4({ 1, 0, 0, 1 }));
    m_renderer->drawSprite(Vector2({ -100, -100 }), Vector2({ 100, 100 }), 0.0f, m_debugTexture, Vector4({ 1, 1, 1, 1 }));
    m_renderer->textFont(FontFactory::getInstance()->load("./assets/NotoSansJP-Regular.ttf"));
    m_renderer->textFontSize(20);
    m_renderer->drawText(Vector2({ 200, 200 }), Renderer::TextAlignX::Center, Renderer::TextAlignY::Center, u"こんにちわ", Vector4({ 1, 1, 1, 1 }));
}

bool DebugScene::tryTransition(std::string& outNextScene)
{
    if (m_editCompleted) {
        outNextScene = "Game";
    }
    return m_editCompleted;
}
// private
}