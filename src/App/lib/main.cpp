#include <Graphics.hpp>
#include <Input.hpp>
#include <cassert>

class Tile {
public:
    explicit Tile(Lib::Graphics::RenderInterface renderInterface)
        : renderParameter(Lib::Graphics::RenderParameter::create(renderInterface))
    {
    }

    std::shared_ptr<Lib::Graphics::RenderParameter> renderParameter;
    Lib::Math::Matrix modelMatrix;
};

int main(int argc, char* argv[])
{
    auto engine = Lib::Graphics::Engine::getInstance()->startup(argc, argv);
    auto device = engine->getDevice();
    auto window = engine->getWindow();
    auto surface = device->getSurface();
    auto texture = Lib::Graphics::Texture::create(L"assets/Sprite.png");
    assert(texture != nullptr);

    auto shader = Lib::Graphics::Shader::compile(R"(
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
    auto rc = Lib::Graphics::RenderContext::create(Lib::Graphics::PrimitiveType::Triangles);
    const std::vector<Lib::Math::Vector3> verts(
        { (Lib::Math::Vector3({ left, bottom, 0 })),
            (Lib::Math::Vector3({ left, top, 0 })),
            (Lib::Math::Vector3({ right, bottom, 0 })),
            (Lib::Math::Vector3({ right, top, 0 })) });
    rc->updateVertex(verts.data(), static_cast<int32_t>(verts.size()));
    rc->updateIndex(index.data(), static_cast<int32_t>(index.size()));

    std::vector<Tile> tiles;
    Lib::Math::Vector3 eyePos = Lib::Math::Vector3({ 0, 0, -5 });
    int32_t globalIndex = 0;
    const float tileSize = 2.0f;
    for (int32_t i = 0; i < 5; i++) {
        float fi = static_cast<float>(i) * tileSize;
        for (int32_t j = 0; j < 5; j++) {
            float fj = static_cast<float>(j) * tileSize;
            auto modelR = Lib::Math::Matrix ::rotateX(Lib::Math::Mathf::Deg2Rad * -90.0f);
            auto modelS = Lib::Math::Matrix::scale(Lib::Math::Vector3({ tileSize, tileSize, 1.0f }));
            auto color = Lib::Math::Vector4({ static_cast<float>(globalIndex) / 25.0f, 0, 0, 1 });
            globalIndex++;

            {
                auto modelT = Lib::Math::Matrix::translate(Lib::Math::Vector3({ fi, -1.0f, fj }));
                Tile tile(Lib::Graphics::RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            }
            {
                auto modelT = Lib::Math::Matrix::translate(Lib::Math::Vector3({ -fi, -1.0f, fj }));
                Tile tile(Lib::Graphics::RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            }
            {
                auto modelT = Lib::Math::Matrix::translate(Lib::Math::Vector3({ fi, -1.0f, -fj }));
                Tile tile(Lib::Graphics::RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            }
            {
                auto modelT = Lib::Math::Matrix::translate(Lib::Math::Vector3({ -fi, -1.0f, -fj }));
                Tile tile(Lib::Graphics::RenderInterface::Color);
                tile.modelMatrix = (modelS * modelR * modelT);
                tile.renderParameter->setColor(color);
                tiles.emplace_back(tile);
            }
        }
    }

    auto controller = Lib::Input::Gamepad::getGamepad(0);

    auto ortho = Lib::Math::Matrix::perspective(90.0f, Lib::Graphics::Screen::getAspectRatio(), 1, 1000);
    auto view = Lib::Math::Matrix::lookAt(
        eyePos,
        Lib::Math::Vector3({ 0, 0, 1 }),
        Lib::Math::Vector3({ 0, 1, 0 }));
    window->show();
    while (true) {
        if (window->translateMessage()) {
            break;
        }
        Lib::Input::Gamepad::sync();

        surface->begin();
        for (auto& tile : tiles) {
            tile.renderParameter->setTransform(tile.modelMatrix * view * ortho);
            surface->draw(shader, tile.renderParameter, rc);
        }
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}