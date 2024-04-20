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
    auto vb = Lib::Graphics::Buffer::create();
    const std::vector<Lib::Math::Vector3> verts(
        { Lib::Math::Vector3({ left, bottom, 0.0f }),
            Lib::Math::Vector3({ left, top, 0.0f }),
            Lib::Math::Vector3({ right, bottom, 0.0f }),
            Lib::Math::Vector3({ right, top, 0.0f }) });
    vb->allocate(sizeof(Lib::Math::Vector3) * verts.size());
    vb->update(verts.data());

    auto ib = Lib::Graphics::Buffer::create();
    const std::vector<uint32_t> index { 0, 1, 2, 2, 1, 3 };
    ib->allocate(sizeof(uint32_t) * index.size());
    ib->update(index.data());

    auto param = std::make_shared<Lib::Graphics::RenderParameter>();
    auto rotate = Lib::Math::Matrix::rotateY(Lib::Math::Mathf::Deg2Rad * 35.0f);
    auto pers = Lib::Math::Matrix::perspective(30.0f, 800.0f / 600.0f, -1, 10);
    auto view = Lib::Math::Matrix::lookAt(
        Lib::Math::Vector3({ 0, 0, -1 }),
        Lib::Math::Vector3({ 0, 0, 1 }),
        Lib::Math::Vector3({ 0, 1, 0 }));
    // auto translate = Lib::Math::Matrix::translate(Lib::Math::Vector3({ 0, 0, 5 }));
    // auto scale = Lib::Math::Matrix::scale(Lib::Math::Vector3({ 10, 10, 1 }));
    param->setTransform(rotate * view * pers);
    param->setColor(Lib::Math::Vector4({ 1, 0, 1, 1 }));
    // param->setTexture(texture);

    Lib::Math::Vector3 pt = Lib::Math::makeVector3(0, 0, 0);
    Lib::Math::Vector3 pt2 = Lib::Math::Matrix::multiply(pers, pt);
    std::cout << pt << " " << pt2 << std::endl;

    window->show();
    while (true) {
        if (window->translateMessage()) {
            break;
        }
        surface->begin();
        surface->draw(shader, param, Lib::Graphics::PrimitiveType::Triangles, 3, false, vb, ib, 6);
        surface->end();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}