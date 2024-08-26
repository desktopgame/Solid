#include <Graphics.hpp>
#include <Input.hpp>
#include <Math.hpp>
#include <cassert>
#include <fstream>
#include <picojson/picojson.h>

using namespace Lib::Graphics;
using namespace Lib::Math;

int main(int argc, char* argv[])
{
    auto engine = Engine::getInstance()->startup(argc, argv);
    auto device = engine->getDevice();
    auto window = engine->getWindow();
    auto surface = device->getSurface();

    auto shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix mat; }
        cbuffer cbuff1 : register(b1) { float4 color; }

        Output vsMain(float2 pos : POSITION) {
            Output output;
            output.svpos = mul(mat, float4(pos, 0, 1));
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
    auto vertexBuffer = Buffer::create();
    auto indexBuffer = Buffer::create();
    std::vector<Vector2> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(Vector2({ left, top }));
    vertices.push_back(Vector2({ right, top }));
    vertices.push_back(Vector2({ right, bottom }));
    vertices.push_back(Vector2({ left, bottom }));
    vertexBuffer->allocate(sizeof(Vector2) * vertices.size());
    vertexBuffer->update(vertices.data());
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(1);
    indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    indexBuffer->update(indices.data());

    auto pso = PipelineStateObject::create(shader, RenderInterface::Color, PrimitiveType::Triangles, 2, false);
    auto renderParameter = RenderParameter::create(RenderInterface::Color);
    Matrix ortho = Matrix::ortho(Screen::getWidth(), Screen::getHeight(), -1.0f, 1.0f);
    Matrix model = Matrix::scale(Vector3({ 100, 100, 1 }));
    renderParameter->setTransform(model * ortho);
    renderParameter->setColor(Vector4({ 1, 0, 0, 1 }));

    auto controller
        = Lib::Input::Gamepad::getGamepad(0);
    window->show();
    while (window->peekMessage()) {
        Lib::Input::Gamepad::sync();

        surface->begin();
        surface->render(pso, renderParameter, vertexBuffer, indexBuffer, indices.size());
        surface->end();

        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}