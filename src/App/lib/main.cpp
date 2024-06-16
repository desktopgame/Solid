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
        std::ifstream ifs("assets\\Stage\\Stage01.json");
        picojson::value stage;
        picojson::parse(stage, ifs);

        picojson::array& tileDatas = stage.get<picojson::object>()["tiles"].get<picojson::array>();
        float tileSize = static_cast<float>(stage.get<picojson::object>()["tileSize"].get<double>());
        auto modelS = Matrix::scale(Vector3({ tileSize, tileSize, 1.0f }));
        for (picojson::value tileData : tileDatas) {
            picojson::object tileObject = tileData.get<picojson::object>();
            float x = static_cast<float>(tileObject["x"].get<double>());
            float y = static_cast<float>(tileObject["y"].get<double>());
            float z = static_cast<float>(tileObject["z"].get<double>());
            float r = static_cast<float>(tileObject["r"].get<double>());
            float g = static_cast<float>(tileObject["g"].get<double>());
            float b = static_cast<float>(tileObject["b"].get<double>());
            std::string n = tileObject["n"].get<std::string>();
            auto modelT = Matrix::translate(Vector3({ x, y, z }));
            auto modelR = Matrix::scale(Vector3({ 1, 1, 1 }));

            if (n == "Y+") {
                modelR = Matrix ::rotateX(Mathf::Deg2Rad * -90.0f);
            } else if (n == "Y-") {
                modelR = Matrix ::rotateX(Mathf::Deg2Rad * 90.0f);
            } else if (n == "X+") {
                modelR = Matrix ::rotateY(Mathf::Deg2Rad * 90.0f);
            } else if (n == "X-") {
                modelR = Matrix ::rotateY(Mathf::Deg2Rad * -90.0f);
            } else if (n == "Z+") {
                modelR = Matrix ::rotateY(Mathf::Deg2Rad * 180.0f);
            } else if (n == "Z-") {
                modelR = Matrix ::scale(Vector3({ 1, 1, 1 }));
            }

            Tile tile(RenderInterface::Color);
            tile.modelMatrix = modelS * modelR * modelT;
            tile.renderParameter->setColor(Vector4({ r, g, b, 1.0f }));
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
        float leftStickX = static_cast<float>(controller->getLeftStickX() / 32768.0f);
        float leftStickY = static_cast<float>(controller->getLeftStickY() / 32768.0f);
        // eyePos.x() += leftStickX * 0.1f;
        float lt = (static_cast<float>(controller->getLeftTrigger()) / 255.0f);
        float rt = (static_cast<float>(controller->getRightTrigger()) / 255.0f);

        float rightStickX = static_cast<float>(controller->getRightStickX() / 32768.0f) * 0.04f;
        float rightStickY = static_cast<float>(controller->getRightStickY() / 32768.0f) * 0.04f;
        if (rightStickX == 0.0f) {
            // eyeAngleX /= 2.0f;
        }
        eyeAngleX += rightStickX;
        if (eyeAngleX < -2.0f) {
            eyeAngleX = -2.0f;
        }
        if (eyeAngleX > 2.0f) {
            eyeAngleX = 2.0f;
        }
        if (rightStickY == 0.0f) {
            // eyeAngleY /= 2.0f;
        }
        eyeAngleY += rightStickY;
        if (eyeAngleY < -0.5f) {
            eyeAngleY = -0.5f;
        }
        if (eyeAngleY > 0.5f) {
            eyeAngleY = 0.5f;
        }
        auto q = Quaternion::angleAxis(-eyeAngleY * 90.0f, Vector3({ 1, 0, 0 })) * Quaternion::angleAxis(eyeAngleX * 90.0f, Vector3({ 0, 1, 0 }));
        auto fwd = Quaternion::transform(q, Vector3({ 0, 0, 1 }));
        auto right = Quaternion::transform(q * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));
        if (lt > 0.0f) {
            eyePos += fwd * Vector3({ 1, 0, 1 });
        } else if (rt > 0.0f) {
            eyePos -= fwd * Vector3({ 1, 0, 1 });
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
            eyePos + fwd,
            Vector3({ 0, 1, 0 }));

        surface->begin();
        for (auto& tile : tiles) {
            tile.renderParameter->setTransform(tile.modelMatrix * view * persp);
            surface->draw(shader, tile.renderParameter, rc);
        }
        // {
        //     auto boxS = Matrix::scale(Vector3({ 10, 10, 10 }));
        //     auto boxT = Matrix::translate(Vector3({ 0, 50, 10 }));
        //     param2->setTransform((boxS * boxT) * view * persp);
        //     surface->draw(shader, param2, rc2);
        // }
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}