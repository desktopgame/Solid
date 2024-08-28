#include <Graphics/Buffer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Surface.hpp>

namespace Lib::Graphics {
// public
Renderer::Renderer()
    : m_rectObject()
{
}

void Renderer::drawRect(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, const Lib::Math::Color& color)
{
    if (m_rectObject.pso == nullptr) {
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
        m_rectObject.vertexBuffer = Buffer::create();
        m_rectObject.indexBuffer = Buffer::create();
        std::vector<Lib::Math::Vector2> vertices;
        std::vector<uint32_t> indices;
        const float left = -0.5;
        const float right = 0.5;
        const float top = 0.5;
        const float bottom = -0.5;
        vertices.push_back(Lib::Math::Vector2({ left, top }));
        vertices.push_back(Lib::Math::Vector2({ right, top }));
        vertices.push_back(Lib::Math::Vector2({ right, bottom }));
        vertices.push_back(Lib::Math::Vector2({ left, bottom }));
        m_rectObject.vertexBuffer->allocate(sizeof(Lib::Math::Vector2) * vertices.size());
        m_rectObject.vertexBuffer->update(vertices.data());
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(1);
        m_rectObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
        m_rectObject.indexBuffer->update(indices.data());
        m_rectObject.indexLength = indices.size();
        m_rectObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 2, false);
    }
    auto constant = Constant::rent(Constant::Layout::Color);
    constant->setTransform(Camera::transform2D(Lib::Math::Matrix::transform(
        Lib::Math::Matrix::translate(Lib::Math::Vector3(position, 0)),
        Lib::Math::Matrix(),
        Lib::Math::Matrix::scale(Lib::Math::Vector3(size, 1.0f)))));
    constant->setColor(color);
    renderObject(m_rectObject, constant);
}
// private
void Renderer::renderObject(const Object& object, const std::shared_ptr<Constant> constant)
{
    Engine::getInstance()->getDevice()->getSurface()->render(
        object.pso,
        constant,
        object.vertexBuffer,
        object.indexBuffer,
        object.indexLength);
}
}