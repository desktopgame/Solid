#include <Graphics/Buffer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Surface.hpp>
#include <Math/Mathf.hpp>

namespace Lib::Graphics {
// public
Renderer::Renderer()
    : m_rectObject()
    , m_circleObject()
    , m_planeObject()
    , m_boxObject()
{
}

void Renderer::drawRect(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, const Color& color)
{
    initRect();
    auto constant = Constant::rent(Constant::Layout::Color);
    constant->setTransform(Camera::transform2D(Lib::Math::Matrix::transform(
        Lib::Math::Matrix::translate(Lib::Math::Vector3(position, 0)),
        Lib::Math::Matrix(),
        Lib::Math::Matrix::scale(Lib::Math::Vector3(size, 1.0f)))));
    constant->setColor(color);
    renderObject(m_rectObject, constant);
}

void Renderer::drawCircle(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, const Color& color)
{
    initCircle();
    auto constant = Constant::rent(Constant::Layout::Color);
    constant->setTransform(Camera::transform2D(Lib::Math::Matrix::transform(
        Lib::Math::Matrix::translate(Lib::Math::Vector3(position, 0)),
        Lib::Math::Matrix(),
        Lib::Math::Matrix::scale(Lib::Math::Vector3(size, 1.0f)))));
    constant->setColor(color);
    renderObject(m_circleObject, constant);
}

void Renderer::drawPlane(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Color& color)
{
    initPlane();
    auto constant = Constant::rent(Constant::Layout::Color);
    constant->setTransform(Camera::transform3D(Lib::Math::Matrix::transform(
        Lib::Math::Matrix::translate(position),
        Lib::Math::Matrix(),
        Lib::Math::Matrix::scale(size))));
    constant->setColor(color);
    renderObject(m_planeObject, constant);
}

void Renderer::drawBox(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Color& color)
{
}
// private
void Renderer::initRect()
{
    if (m_rectObject.pso != nullptr) {
        return;
    }
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

void Renderer::initCircle()
{
    if (m_circleObject.pso != nullptr) {
        return;
    }
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
    m_circleObject.vertexBuffer = Buffer::create();
    m_circleObject.indexBuffer = Buffer::create();
    std::vector<Lib::Math::Vector2> vertices;
    std::vector<uint32_t> indices;
    float degree = 0.0f;
    vertices.emplace_back(Lib::Math::Vector2({ 0.0f, 0.0f }));
    while (degree < 360.0f) {
        float x = Lib::Math::Mathf::cos(Lib::Math::Mathf::Deg2Rad * degree) * 0.5f;
        float y = Lib::Math::Mathf::sin(Lib::Math::Mathf::Deg2Rad * degree) * 0.5f;
        vertices.emplace_back(Lib::Math::Vector2({ x, y }));
        degree += 5.0f;
    }
    m_circleObject.vertexBuffer->allocate(sizeof(Lib::Math::Vector2) * vertices.size());
    m_circleObject.vertexBuffer->update(vertices.data());
    uint32_t index = 1;
    while (index < vertices.size()) {
        indices.emplace_back(0);
        indices.emplace_back(index + 1);
        indices.emplace_back(index);
        index = index + 1;
    }
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(index - 1);
    m_circleObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_circleObject.indexBuffer->update(indices.data());
    m_circleObject.indexLength = indices.size();
    m_circleObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 2, false);
}

void Renderer::initPlane()
{
    if (m_planeObject.pso != nullptr) {
        return;
    }
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
    m_planeObject.vertexBuffer = Buffer::create();
    m_planeObject.indexBuffer = Buffer::create();
    std::vector<Lib::Math::Vector3> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.emplace_back(Lib::Math::Vector3({ left, bottom, 0 }));
    vertices.emplace_back(Lib::Math::Vector3({ left, top, 0 }));
    vertices.emplace_back(Lib::Math::Vector3({ right, bottom, 0 }));
    vertices.emplace_back(Lib::Math::Vector3({ right, top, 0 }));
    m_planeObject.vertexBuffer->allocate(sizeof(Lib::Math::Vector3) * vertices.size());
    m_planeObject.vertexBuffer->update(vertices.data());
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
    m_planeObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_planeObject.indexBuffer->update(indices.data());
    m_planeObject.indexLength = indices.size();
    m_planeObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 3, false);
}

void Renderer::initBox()
{
    if (m_boxObject.pso != nullptr) {
        return;
    }
}

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