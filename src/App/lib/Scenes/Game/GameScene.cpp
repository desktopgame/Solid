#include <Common/Constants.hpp>
#include <Scenes/Game/GameScene.hpp>
#include <cmath>
#include <imgui.h>

namespace App::Scenes::Game {
// public
GameScene::GameScene()
    : m_cameraPos({ 0, (k_tileSize / 2.0f) + 1.0f, -1 })
    , m_cameraLookAt({ 0, 0, 0 })
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(0.4f)
    , m_cameraRotateSpeed(0.8f)
    , m_globalLightDir({ 1, 1, 0 })
{
}
GameScene::~GameScene() { }

void GameScene::onEnter()
{
    if (!m_renderer) {
        m_renderer = std::make_shared<Renderer>();
    }
    if (!m_debugTexture) {
        m_debugTexture = Texture::create("./assets/tileNormal2.png");
    }
    if (!m_borderTexture) {
        m_borderTexture = Texture::create("./assets/tileBorder.png");
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
        IO::deserializeTile("assets\\Stages\\stage_base.csv", instances, 1.0f);
        instBuf->allocate(sizeof(Vector4) * instances.size());
        instBuf->update(instances.data());
        m_instanceBuffers.emplace_back(instBuf);
        m_instanceCount = static_cast<int32_t>(instances.size());

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
        if (Cursor::isVisible()) {
            Cursor::hide();
            Cursor::lock(Engine::getInstance()->getWindow());
        } else {
            Cursor::show();
            Cursor::unlock();
        }
    }
}

void GameScene::onGui()
{
    ImGui::Begin("Light");
    ImGui::DragFloat3("GlobalLight", m_globalLightDir.data(), 0.001f, -1.0f, 1.0f);
    ImGui::End();
}

void GameScene::onDraw3D()
{
    Camera::position(m_cameraPos);
    Camera::lookAt(m_cameraLookAt);
    Camera::depthRange(0.1f, 1000.0f);

    GlobalLight::enable();
    GlobalLight::set(Vector3::normalized(m_globalLightDir));

    PointLight::enable();
    PointLight::setCount(3);
    PointLight::set(0, Vector3({ 120, 10, 120 }), 10, 20);
    PointLight::set(1, Vector3({ 90, 10, 90 }), 10, 20);
    PointLight::set(2, Vector3({ 60, 10, 60 }), 10, 20);

    auto rc = RenderContext::get(Metadata::ProgramTable::TileInstance3D);
    auto ub = UniformPool::rent(Metadata::ProgramTable::TileInstance3D);
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = Matrix::transform(
        Matrix::translate(Vector3({ 0, 0, 0 })),
        Matrix(),
        Matrix::scale(Vector3({ 5, 5, 5 })));
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);
    ub->setVS(1, &m_tileTransform);
    ub->setVS(2, &m_tilePallet);

    Reflect::UVector3 uCameraPos;
    uCameraPos.value = Camera::getPosition();
    ub->setVS(3, &uCameraPos);

    ub->setPS(0, m_debugTexture);
    ub->setPS(1, m_borderTexture);
    Engine::getInstance()->getDevice()->getSurface()->render(
        rc,
        ub,
        m_vertexBuffer,
        m_indexBuffer,
        m_indexLength,
        m_instanceBuffers,
        m_instanceCount);
}

void GameScene::onDraw2D()
{
}

bool GameScene::tryTransition(std::string& outNextScene)
{
    return false;
}
// private
}