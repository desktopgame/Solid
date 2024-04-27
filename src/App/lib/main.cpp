#include <Graphics.hpp>
#include <Input.hpp>
#include <cassert>
#include <functional>

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

void eachAxisY(const Vector3& axis, const std::function<void(Vector3)>& cb)
{
    cb(Vector3({ axis.x(), axis.y(), axis.z() }));
    cb(Vector3({ -axis.x(), axis.y(), axis.z() }));
    cb(Vector3({ axis.x(), axis.y(), -axis.z() }));
    cb(Vector3({ -axis.x(), axis.y(), -axis.z() }));
    // x = 0
    cb(Vector3({ 0, axis.y(), axis.z() }));
    cb(Vector3({ 0, axis.y(), -axis.z() }));
    // z = 0
    cb(Vector3({ axis.x(), axis.y(), 0 }));
    cb(Vector3({ -axis.x(), axis.y(), 0 }));
}

void eachAxisRight(const Vector3& axis, const std::function<void(Vector3)>& cb)
{
    cb(Vector3({ axis.x(), axis.y(), axis.z() }));
    cb(Vector3({ axis.x(), axis.y(), -axis.z() }));
    cb(Vector3({ axis.x(), 0.0f, axis.z() }));
    cb(Vector3({ axis.x(), 0.0f, -axis.z() }));
}

void eachAxisLeft(const Vector3& axis, const std::function<void(Vector3)>& cb)
{
    cb(Vector3({ -axis.x(), axis.y(), axis.z() }));
    cb(Vector3({ -axis.x(), axis.y(), -axis.z() }));
    cb(Vector3({ -axis.x(), 0.0f, axis.z() }));
    cb(Vector3({ -axis.x(), 0.0f, -axis.z() }));
}

void eachAxisForward(const Vector3& axis, const std::function<void(Vector3)>& cb)
{
    cb(Vector3({ axis.x(), axis.y(), axis.z() }));
    cb(Vector3({ -axis.x(), axis.y(), axis.z() }));
    cb(Vector3({ axis.x(), 0.0f, axis.z() }));
    cb(Vector3({ -axis.x(), 0.0f, axis.z() }));
}

void eachAxisBackward(const Vector3& axis, const std::function<void(Vector3)>& cb)
{
    cb(Vector3({ axis.x(), axis.y(), -axis.z() }));
    cb(Vector3({ -axis.x(), axis.y(), -axis.z() }));
    cb(Vector3({ axis.x(), 0.0f, -axis.z() }));
    cb(Vector3({ -axis.x(), 0.0f, -axis.z() }));
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
    const float tileSize = 2.0f;
    for (int32_t i = 0; i < 5; i++) {
        float fi = static_cast<float>(i + 1) * tileSize;
        for (int32_t j = 0; j < 5; j++) {
            float fj = static_cast<float>(j + 1) * tileSize;
            auto modelR = Matrix ::rotateX(Mathf::Deg2Rad * -90.0f);
            auto modelS = Matrix::scale(Vector3({ tileSize, tileSize, 1.0f }));
            auto color = Vector4({ static_cast<float>(i) / 5.0f, static_cast<float>(j) / 5.0f, 0.5f, 1 });

            eachAxisY(Vector3({ fi, -1.0f, fj }), [&](Vector3 t) -> void {
                auto modelT = Matrix::translate(t);
                Tile tile(RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            });

            modelR = Matrix ::rotateX(Mathf::Deg2Rad * 90.0f);
            eachAxisY(Vector3({ fi, tileSize * 5, fj }), [&](Vector3 t) -> void {
                auto modelT = Matrix::translate(t);
                Tile tile(RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            });

            modelR = Matrix ::rotateY(Mathf::Deg2Rad * -90.0f);
            eachAxisRight(Vector3({ tileSize * 5.0f, fj - 1.0f, fi }), [&](Vector3 t) -> void {
                auto modelT = Matrix::translate(t);
                Tile tile(RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            });

            modelR = Matrix ::rotateY(Mathf::Deg2Rad * 90.0f);
            eachAxisLeft(Vector3({ tileSize * 5.0f, fj - 1.0f, fi }), [&](Vector3 t) -> void {
                auto modelT = Matrix::translate(t);
                Tile tile(RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            });

            modelR = Matrix ::rotateY(0.0f);
            eachAxisForward(Vector3({ fi, fj - 1.0f, tileSize * 5.0f }), [&](Vector3 t) -> void {
                auto modelT = Matrix::translate(t);
                Tile tile(RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            });

            modelR = Matrix ::rotateY(Mathf::Deg2Rad * 180.0f);
            eachAxisBackward(Vector3({ fi, fj - 1.0f, tileSize * 5.0f }), [&](Vector3 t) -> void {
                auto modelT = Matrix::translate(t);
                Tile tile(RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            });
        }
    }

    {
        auto modelR = Matrix ::rotateX(Mathf::Deg2Rad * -90.0f);
        auto modelS = Matrix::scale(Vector3({ tileSize, tileSize, 1.0f }));
        auto modelT = Matrix::translate(Vector3({ 0.0f, -1.0f, 0.0f }));
        Tile tile(RenderInterface::Color);
        tile.modelMatrix = (modelS * modelR * modelT);
        tile.renderParameter->setColor(Vector4({ 0, 0, 0.5f, 1.0f }));
        tiles.emplace_back(tile);
    }

    auto controller = Lib::Input::Gamepad::getGamepad(0);

    auto eyePos = Vector3({ 0, 1, -5 });
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
        // eyePos.x() += leftStickX * 0.1f;
        if (controller->getLeftTrigger() > 0) {
            eyePos += Vector3(
                {
                    Mathf::cos(Mathf::Deg2Rad * (eyeAngleX - 1.0f) * 90.0f), //
                    0, //
                    -Mathf::sin(Mathf::Deg2Rad * (eyeAngleX - 1.0f) * 90.0f) //
                });
        } else if (controller->getRightTrigger() > 0) {
            eyePos -= Vector3(
                {
                    Mathf::cos(Mathf::Deg2Rad * (eyeAngleX - 1.0f) * 90.0f), //
                    0, //
                    -Mathf::sin(Mathf::Deg2Rad * (eyeAngleX - 1.0f) * 90.0f) //
                });
        } else if (leftStickX > 0.5f) {
            eyePos += Vector3(
                {
                    Mathf::cos(Mathf::Deg2Rad * (eyeAngleX - 0.0f) * 90.0f), //
                    0, //
                    -Mathf::sin(Mathf::Deg2Rad * (eyeAngleX - 0.0f) * 90.0f) //
                });
        } else if (leftStickX < -0.5f) {
            eyePos -= Vector3(
                {
                    Mathf::cos(Mathf::Deg2Rad * (eyeAngleX - 0.0f) * 90.0f), //
                    0, //
                    -Mathf::sin(Mathf::Deg2Rad * (eyeAngleX - 0.0f) * 90.0f) //
                });
        }

        float rightStickX = static_cast<float>(controller->getRightStickX() / 32768.0f) * 0.1f;
        float rightStickY = static_cast<float>(controller->getRightStickY() / 32768.0f) * 0.1f;
        eyeAngleX += rightStickX;
        if (eyeAngleX < -0.5f) {
            eyeAngleX = -0.5f;
        }
        if (eyeAngleX > 0.5f) {
            eyeAngleX = 0.5f;
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
                0, //
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