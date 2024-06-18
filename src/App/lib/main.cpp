#include <Graphics.hpp>
#include <Input.hpp>
#include <Math.hpp>
#include <cassert>
#include <fstream>
#include <picojson/picojson.h>

using namespace Lib::Graphics;
using namespace Lib::Math;

class Tile {
public:
    explicit Tile(RenderInterface renderInterface)
        : renderParameter(RenderParameter::create(renderInterface))
    {
    }

    std::shared_ptr<RenderParameter> renderParameter;
    Matrix modelMatrix;
};

void primPlane(std::vector<Vector3>& verts, std::vector<uint32_t>& index)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    verts.emplace_back(Vector3({ left, bottom, 0 }));
    verts.emplace_back(Vector3({ left, top, 0 }));
    verts.emplace_back(Vector3({ right, bottom, 0 }));
    verts.emplace_back(Vector3({ right, top, 0 }));
    index.emplace_back(0);
    index.emplace_back(1);
    index.emplace_back(2);
    index.emplace_back(2);
    index.emplace_back(1);
    index.emplace_back(3);
}

void primBox(std::vector<Vector3>& verts, std::vector<uint32_t>& index)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    const float forward = 0.5f;
    const float backward = -0.5f;
    verts.emplace_back(Vector3({ left, bottom, backward }));
    verts.emplace_back(Vector3({ left, top, backward }));
    verts.emplace_back(Vector3({ right, bottom, backward }));
    verts.emplace_back(Vector3({ right, top, backward }));
    verts.emplace_back(Vector3({ left, bottom, forward }));
    verts.emplace_back(Vector3({ left, top, forward }));
    verts.emplace_back(Vector3({ right, bottom, forward }));
    verts.emplace_back(Vector3({ right, top, forward }));
    // backward
    index.emplace_back(0);
    index.emplace_back(1);
    index.emplace_back(2);
    index.emplace_back(2);
    index.emplace_back(1);
    index.emplace_back(3);
    // left
    index.emplace_back(4);
    index.emplace_back(5);
    index.emplace_back(1);
    index.emplace_back(4);
    index.emplace_back(1);
    index.emplace_back(0);
    // right
    index.emplace_back(2);
    index.emplace_back(3);
    index.emplace_back(6);
    index.emplace_back(3);
    index.emplace_back(7);
    index.emplace_back(6);
    // forward
    index.emplace_back(6);
    index.emplace_back(7);
    index.emplace_back(4);
    index.emplace_back(4);
    index.emplace_back(7);
    index.emplace_back(5);
    // top
    index.emplace_back(1);
    index.emplace_back(5);
    index.emplace_back(3);
    index.emplace_back(5);
    index.emplace_back(7);
    index.emplace_back(3);
    // bottom
    index.emplace_back(4);
    index.emplace_back(0);
    index.emplace_back(2);
    index.emplace_back(4);
    index.emplace_back(2);
    index.emplace_back(6);
}

int main(int argc, char* argv[])
{
    auto engine = Engine::getInstance()->startup(argc, argv);
    auto device = engine->getDevice();
    auto window = engine->getWindow();
    auto surface = device->getSurface();
    auto texture = Texture::create(L"assets/Sprite.png");
    assert(texture != nullptr);

    auto shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix mat; }
        cbuffer cbuff1 : register(b1) { float4 color; }

        Output vsMain(float3 pos : POSITION) {
            Output output;
            output.svpos = mul(mat, float4(pos, 1));
            output.color = color;
            return output;
        })",
        "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };

        float4 psMain(Output input) : SV_TARGET {
            return input.color;
        })",
        "psMain");
    std::vector<Vector3> planeVerts;
    std::vector<uint32_t> planeIndex;
    auto rc = RenderContext::create(PrimitiveType::Triangles);
    primPlane(planeVerts, planeIndex);
    rc->updateVertex(planeVerts.data(), static_cast<int32_t>(planeVerts.size()));
    rc->updateIndex(planeIndex.data(), static_cast<int32_t>(planeIndex.size()));

    std::vector<Vector3> boxVerts;
    std::vector<uint32_t> boxIndex;
    auto rc2 = RenderContext::create(PrimitiveType::Triangles);
    primBox(boxVerts, boxIndex);
    rc2->updateVertex(boxVerts.data(), static_cast<int32_t>(boxVerts.size()));
    rc2->updateIndex(boxIndex.data(), static_cast<int32_t>(boxIndex.size()));
    auto param2 = RenderParameter::create(RenderInterface::Color);
    param2->setColor(Vector4({ 1.0f, 0, 0, 1.0f }));

    std::vector<Tile> tiles;
    {
        float size = 1000.0f;
        Matrix modelScale = Matrix::scale(Vector3({ size, size, 1 }));
        Matrix modelPosY = Matrix ::rotateX(Mathf::Deg2Rad * -90.0f);
        Matrix modelNegY = Matrix ::rotateX(Mathf::Deg2Rad * 90.0f);
        Matrix modelPosX = Matrix ::rotateY(Mathf::Deg2Rad * 90.0f);
        Matrix modelNegX = Matrix ::rotateY(Mathf::Deg2Rad * -90.0f);
        Matrix modelPosZ = Matrix ::rotateY(Mathf::Deg2Rad * 180.0f);
        Matrix modelNegZ = Matrix ::scale(Vector3({ 1, 1, 1 }));

        {
            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelScale * modelPosY * Matrix::translate(Vector3({ 0, -size / 2.0f, 0 }));
            tile.renderParameter->setColor(Vector4({ 1, 1, 1, 1 }));
            tiles.emplace_back(tile);
        }
        {
            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelScale * modelNegY * Matrix::translate(Vector3({ 0, size / 2.0f, 0 }));
            tile.renderParameter->setColor(Vector4({ 1, 1, 1, 1 }));
            tiles.emplace_back(tile);
        }

        {
            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelScale * modelPosZ * Matrix::translate(Vector3({ 0, 0, -size / 2.0f }));
            tile.renderParameter->setColor(Vector4({ 0.25f, 0.8f, 0.25f, 1 }));
            tiles.emplace_back(tile);
        }
        {
            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelScale * modelNegZ * Matrix::translate(Vector3({ 0, 0, size / 2.0f }));
            tile.renderParameter->setColor(Vector4({ 0.25f, 0.8f, 0.25f, 1 }));
            tiles.emplace_back(tile);
        }

        {
            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelScale * modelPosX * Matrix::translate(Vector3({ -size / 2.0f, 0, 0 }));
            tile.renderParameter->setColor(Vector4({ 0.25f, 0.8f, 0.25f, 1 }));
            tiles.emplace_back(tile);
        }
        {
            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelScale * modelNegX * Matrix::translate(Vector3({ size / 2.0f, 0, 0 }));
            tile.renderParameter->setColor(Vector4({ 0.25f, 0.8f, 0.25f, 1 }));
            tiles.emplace_back(tile);
        }
    }

    auto controller = Lib::Input::Gamepad::getGamepad(0);

    auto eyePos = Vector3({ 0, 1, -1 });
    auto persp = Matrix::perspective(90.0f, Screen::getAspectRatio(), 1, 1000);
    float eyeAngleX = 0.0f;
    float eyeAngleY = 0.0f;
    window->show();
    while (true) {
        if (window->translateMessage()) {
            break;
        }
        Lib::Input::Gamepad::sync();
        float leftStickX = static_cast<float>(controller->getLeftStickX()) / 32768.0f;
        float leftStickY = static_cast<float>(controller->getLeftStickY()) / 32768.0f;

        float lt = (static_cast<float>(controller->getLeftTrigger()) / 255.0f);
        float rt = (static_cast<float>(controller->getRightTrigger()) / 255.0f);

        float rightStickX = static_cast<float>(controller->getRightStickX()) / 32768.0f;
        float rightStickY = static_cast<float>(controller->getRightStickY()) / 32768.0f;
        eyeAngleX += rightStickY * 3.0f;
        eyeAngleY += rightStickX * 10.0f;

        auto rotation = Quaternion::angleAxis(eyeAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-eyeAngleX, Vector3({ 1, 0, 0 }));
        auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
        auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));
        if (lt > 0.0f) {
            eyePos += forward * Vector3({ 1, 0, 1 });
        } else if (rt > 0.0f) {
            eyePos -= forward * Vector3({ 1, 0, 1 });
        }
        if (leftStickX > 0.5f) {
            eyePos += right * Vector3({ 1, 0, 1 });
        } else if (leftStickX < -0.5f) {
            eyePos -= right * Vector3({ 1, 0, 1 });
        } else if (leftStickY > 0) {
            eyePos += Vector3(
                {
                    0, //
                    4.0f * leftStickY, //
                    0 //
                });
        } else if (leftStickY < 0) {
            eyePos -= Vector3(
                {
                    0, //
                    4.0f * -leftStickY, //
                    0 //
                });
        }

        auto view = Matrix::lookAt(
            eyePos,
            eyePos + forward,
            Vector3({ 0, 1, 0 }));

        surface->begin();
        for (auto& tile : tiles) {
            tile.renderParameter->setTransform(tile.modelMatrix * view * persp);
            surface->render(shader, tile.renderParameter, rc);
        }
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}