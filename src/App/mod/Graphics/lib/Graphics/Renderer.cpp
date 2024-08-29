#include <Graphics/Buffer.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Surface.hpp>
#include <Math/Mathf.hpp>

namespace Lib::Graphics {
// public
Renderer::Renderer()
    : m_position({ 0, 0, -1 })
    , m_lookAt({ 0, 0, 0 })
    , m_zNear(1.0f)
    , m_zFar(1000.0f)
    , m_fovY(30.0f)
    , m_dirtyOrthoMatrix(true)
    , m_dirtyViewMatrix(true)
    , m_dirtyProjectionMatrix(true)
    , m_orthoMatrix()
    , m_viewMatrix()
    , m_projectionMatrix()
    , m_rectObject()
    , m_circleObject()
    , m_planeObject()
    , m_boxObject()
{
}

void Renderer::position(const Lib::Math::Vector3& position)
{
    m_position = position;
    m_dirtyViewMatrix = true;
}

void Renderer::lookAt(const Lib::Math::Vector3& lookAt)
{
    m_lookAt = lookAt;
    m_dirtyViewMatrix = true;
}

void Renderer::depthRange(float zNear, float zFar)
{
    m_zNear = zNear;
    m_zFar = zFar;
    m_dirtyProjectionMatrix = true;
}

void Renderer::fovY(float fovY)
{
    m_fovY = fovY;
    m_dirtyProjectionMatrix = true;
}

void Renderer::drawRect(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, const Color& color)
{
    initRect();
    auto constant = Constant::rent(Constant::Layout::Color);
    constant->setTransform(transform2D(Lib::Math::Matrix::transform(
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
    constant->setTransform(transform2D(Lib::Math::Matrix::transform(
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
    constant->setTransform(transform3D(Lib::Math::Matrix::transform(
        Lib::Math::Matrix::translate(position),
        Lib::Math::Matrix(),
        Lib::Math::Matrix::scale(size))));
    constant->setColor(color);
    renderObject(m_planeObject, constant);
}

void Renderer::drawBox(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Color& color)
{
    initBox();
    auto constant = Constant::rent(Constant::Layout::Color);
    constant->setTransform(transform3D(Lib::Math::Matrix::transform(
        Lib::Math::Matrix::translate(position),
        Lib::Math::Matrix(),
        Lib::Math::Matrix::scale(size))));
    constant->setColor(color);
    renderObject(m_boxObject, constant);
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
    m_boxObject.vertexBuffer = Buffer::create();
    m_boxObject.indexBuffer = Buffer::create();
    std::vector<Lib::Math::Vector3> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    const float forward = 0.5f;
    const float backward = -0.5f;
    vertices.emplace_back(Lib::Math::Vector3({ left, bottom, backward }));
    vertices.emplace_back(Lib::Math::Vector3({ left, top, backward }));
    vertices.emplace_back(Lib::Math::Vector3({ right, bottom, backward }));
    vertices.emplace_back(Lib::Math::Vector3({ right, top, backward }));
    vertices.emplace_back(Lib::Math::Vector3({ left, bottom, forward }));
    vertices.emplace_back(Lib::Math::Vector3({ left, top, forward }));
    vertices.emplace_back(Lib::Math::Vector3({ right, bottom, forward }));
    vertices.emplace_back(Lib::Math::Vector3({ right, top, forward }));
    m_boxObject.vertexBuffer->allocate(sizeof(Lib::Math::Vector3) * vertices.size());
    m_boxObject.vertexBuffer->update(vertices.data());
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
    // left
    indices.emplace_back(4);
    indices.emplace_back(5);
    indices.emplace_back(1);
    indices.emplace_back(4);
    indices.emplace_back(1);
    indices.emplace_back(0);
    // right
    indices.emplace_back(2);
    indices.emplace_back(3);
    indices.emplace_back(6);
    indices.emplace_back(3);
    indices.emplace_back(7);
    indices.emplace_back(6);
    // forward
    indices.emplace_back(6);
    indices.emplace_back(7);
    indices.emplace_back(4);
    indices.emplace_back(4);
    indices.emplace_back(7);
    indices.emplace_back(5);
    // top
    indices.emplace_back(1);
    indices.emplace_back(5);
    indices.emplace_back(3);
    indices.emplace_back(5);
    indices.emplace_back(7);
    indices.emplace_back(3);
    // bottom
    indices.emplace_back(4);
    indices.emplace_back(0);
    indices.emplace_back(2);
    indices.emplace_back(4);
    indices.emplace_back(2);
    indices.emplace_back(6);
    m_boxObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_boxObject.indexBuffer->update(indices.data());
    m_boxObject.indexLength = indices.size();
    m_boxObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 3, false);
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

Lib::Math::Matrix Renderer::transform2D(const Lib::Math::Matrix& m)
{
    if (m_dirtyOrthoMatrix) {
        m_dirtyOrthoMatrix = false;
        m_orthoMatrix = Lib::Math::Matrix::ortho(
            static_cast<float>(Screen::getWidth()),
            static_cast<float>(Screen::getHeight()));
    }
    return m * m_orthoMatrix;
}

Lib::Math::Matrix Renderer::transform3D(const Lib::Math::Matrix& m)
{
    if (m_dirtyViewMatrix) {
        m_dirtyViewMatrix = false;
        m_viewMatrix = Lib::Math::Matrix::lookAt(m_position, m_lookAt, Lib::Math::Vector3({ 0, 1, 0 }));
    }
    if (m_dirtyProjectionMatrix) {
        m_dirtyProjectionMatrix = false;
        m_projectionMatrix = Lib::Math::Matrix::perspective(m_fovY,
            static_cast<float>(Screen::getWidth()) / static_cast<float>(Screen::getHeight()),
            m_zNear,
            m_zFar);
    }
    return m * m_viewMatrix * m_projectionMatrix;
}
}