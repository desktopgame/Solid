#include <Graphics.hpp>
#include <cassert>

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
            float2 uv : TEXCOORD;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix mat; }
        cbuffer cbuff1 : register(b1) { float4 color; }

        Output vsMain(float2 pos : POSITION, float2 uv : TEXCOORD) {
            Output output;
            output.svpos = mul(mat, float4(pos, 0, 1));
            output.uv = uv;
            output.color = color;
            return output;
        })",
        "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 uv : TEXCOORD;
            float4 color : COLOR;
        };

        Texture2D<float4> tex : register(t0);
        SamplerState smp : register(s0);

        float4 psMain(Output input) : SV_TARGET {
            float4 col = float4(tex.Sample(smp, input.uv)) * input.color;
            return col;
        })",
        "psMain");
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    auto vb = Lib::Graphics::Buffer::create();
    const std::vector<Lib::Graphics::VertexData2D> verts(
        { Lib::Graphics::VertexData2D(Lib::Math::Vector2({ left, bottom }), Lib::Math::Vector2({ 0.0f, 1.0f })),
            Lib::Graphics::VertexData2D(Lib::Math::Vector2({ left, top }), Lib::Math::Vector2({ 0.0f, 0.0f })),
            Lib::Graphics::VertexData2D(Lib::Math::Vector2({ right, bottom }), Lib::Math::Vector2({ 1.0f, 1.0f })),
            Lib::Graphics::VertexData2D(Lib::Math::Vector2({ right, top }), Lib::Math::Vector2({ 1.0f, 0.0f })) });
    vb->allocate(sizeof(Lib::Graphics::VertexData2D) * verts.size());
    vb->update(verts.data());

    auto ib = Lib::Graphics::Buffer::create();
    const std::vector<uint32_t> index { 0, 1, 2, 2, 1, 3 };
    ib->allocate(sizeof(uint32_t) * index.size());
    ib->update(index.data());

    auto param = std::make_shared<Lib::Graphics::RenderParameter>();
    auto ortho = Lib::Math::Matrix::ortho(800, 600, -1, 1);
    auto view = Lib::Math::Matrix::lookAt(
        Lib::Math::Vector3({ 0, 0, -1 }),
        Lib::Math::Vector3({ 0, 0, 1 }),
        Lib::Math::Vector3({ 0, 1, 0 }));
    auto scale = Lib::Math::Matrix::scale(Lib::Math::Vector3({ 100, 100, 1 }));
    param->setTransform(scale * view * ortho);
    param->setColor(Lib::Math::Vector4({ 1, 1, 1, 1 }));
    param->setTexture(texture);

    window->show();
    while (true) {
        if (window->translateMessage()) {
            break;
        }
        surface->begin();
        surface->draw(shader, param, Lib::Graphics::PrimitiveType::Triangles, 2, true, vb, ib, 6);
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}