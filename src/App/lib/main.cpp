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
        };
        cbuffer cbuff0 : register(b0) { matrix mat; }

        Output vsMain(float3 pos : POSITION, float2 uv : TEXCOORD) {
            Output output;
            output.svpos = mul(mat, float4(pos, 1));
            output.uv = uv;
            return output;
        })",
        "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 uv : TEXCOORD;
        };

        Texture2D<float4> tex : register(t0);
        SamplerState smp : register(s0);

        float4 psMain(Output input) : SV_TARGET {
            float4 col = float4(tex.Sample(smp, input.uv));
            return col;
        })",
        "psMain");
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    auto vb = Lib::Graphics::Buffer::create();
    const std::vector<Lib::Graphics::VertexData3D> verts(
        { Lib::Graphics::VertexData3D(Lib::Math::Vector3({ left, bottom, 0 }), Lib::Math::Vector2({ 0.0f, 1.0f })),
            Lib::Graphics::VertexData3D(Lib::Math::Vector3({ left, top, 0 }), Lib::Math::Vector2({ 0.0f, 0.0f })),
            Lib::Graphics::VertexData3D(Lib::Math::Vector3({ right, bottom, 0 }), Lib::Math::Vector2({ 1.0f, 1.0f })),
            Lib::Graphics::VertexData3D(Lib::Math::Vector3({ right, top, 0 }), Lib::Math::Vector2({ 1.0f, 0.0f })) });
    vb->allocate(sizeof(Lib::Graphics::VertexData3D) * verts.size());
    vb->update(verts.data());

    auto ib = Lib::Graphics::Buffer::create();
    const std::vector<uint32_t> index { 0, 1, 2, 2, 1, 3 };
    ib->allocate(sizeof(uint32_t) * index.size());
    ib->update(index.data());

    auto param = std::make_shared<Lib::Graphics::RenderParameter>();
    auto rotate = Lib::Math::Matrix::rotateY(Lib::Math::Mathf::Deg2Rad * 45.0f);
    auto pers = Lib::Math::Matrix::perspective(90.0f, Lib::Graphics::Screen::getAspectRatio(), 1, 10);
    auto view = Lib::Math::Matrix::lookAt(
        Lib::Math::Vector3({ 0, 0, -5 }),
        Lib::Math::Vector3({ 0, 0, 1 }),
        Lib::Math::Vector3({ 0, 1, 0 }));
    // auto translate = Lib::Math::Matrix::translate(Lib::Math::Vector3({ 0, 0, 5 }));
    // auto scale = Lib::Math::Matrix::scale(Lib::Math::Vector3({ 10, 10, 1 }));
    param->setTransform(rotate * view * pers);
    // param->setColor(Lib::Math::Vector4({ 1, 0, 1, 1 }));
    param->setTexture(texture);

    Lib::Math::Vector3 pt = Lib::Math::makeVector3(0, 0, 0);
    Lib::Math::Vector3 pt2 = Lib::Math::Matrix::multiply(view * pers, pt);
    std::cout << pt << " " << pt2 << std::endl;

    window->show();
    float deg = 45.0f;
    while (true) {
        if (window->translateMessage()) {
            break;
        }
        surface->begin();
        deg += 1.0f;
        if (deg >= 360.0f) {
            deg -= 360.0f;
        }
        rotate = Lib::Math::Matrix::rotateY(Lib::Math::Mathf::Deg2Rad * deg);
        param->setTransform(rotate * view * pers);

        surface->draw(shader, param, Lib::Graphics::PrimitiveType::Triangles, 3, true, vb, ib, 6);
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}