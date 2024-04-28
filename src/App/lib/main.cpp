#include <Graphics.hpp>
#include <Input.hpp>
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
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    const std::vector<uint32_t> index { 0, 1, 2, 2, 1, 3 };
    auto rc = RenderContext::create(PrimitiveType::Triangles);
    const std::vector<Vector3> verts(
        { (Vector3({ left, bottom, 0 })),
            (Vector3({ left, top, 0 })),
            (Vector3({ right, bottom, 0 })),
            (Vector3({ right, top, 0 })) });
    rc->updateVertex(verts.data(), static_cast<int32_t>(verts.size()));
    rc->updateIndex(index.data(), static_cast<int32_t>(index.size()));

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

    auto eyePos = Vector3({ 0, 1, -205 });
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
        eyePos += Vector3({ 0, 0, 1.1f * (1.0f + (static_cast<float>(controller->getLeftTrigger()) / 255.0f)) });
        if (leftStickX > 0.5f) {
            eyePos += Vector3({ 4.0f, 0, 0 });
        } else if (leftStickX < -0.5f) {
            eyePos -= Vector3({ 4.0f, 0, 0 });
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

        float rightStickX = static_cast<float>(controller->getRightStickX() / 32768.0f) * 0.04f;
        float rightStickY = static_cast<float>(controller->getRightStickY() / 32768.0f) * 0.04f;
        if (rightStickX == 0.0f) {
            eyeAngleX /= 2.0f;
        }
        eyeAngleX += rightStickX;
        if (eyeAngleX < -0.5f) {
            eyeAngleX = -0.5f;
        }
        if (eyeAngleX > 0.5f) {
            eyeAngleX = 0.5f;
        }
        if (rightStickY == 0.0f) {
            eyeAngleY /= 2.0f;
        }
        eyeAngleY += rightStickY;
        if (eyeAngleY < -1.0f) {
            eyeAngleY = -1.0f;
        }
        if (eyeAngleY > 1.0f) {
            eyeAngleY = 1.0f;
        }
        auto view = Matrix::lookAt(
            eyePos,
            eyePos + Vector3({
                Mathf::cos(Mathf::Deg2Rad * (eyeAngleX - 1.0f) * 90.0f), //
                eyeAngleY, //
                -Mathf::sin(Mathf::Deg2Rad * (eyeAngleX - 1.0f) * 90.0f) //
            }),
            Vector3({ 0, 1, 0 }));

        surface->begin();
        for (auto& tile : tiles) {
            tile.renderParameter->setTransform(tile.modelMatrix * view * persp);
            surface->draw(shader, tile.renderParameter, rc);
        }
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}