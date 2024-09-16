#include <Graphics/Buffer.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/FontSprite.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/TileBatch.hpp>
#include <Graphics/VertexNormal3D.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Math/Mathf.hpp>

namespace Lib::Graphics {
// public
Renderer::Renderer()
    : m_position({ 0, 0, -1 })
    , m_lookAt({ 0, 0, 0 })
    , m_zNear(0.1f)
    , m_zFar(100.0f)
    , m_fovY(90.0f)
    , m_lightEnable(false)
    , m_lightDirection({ 0, 1, 0 })
    , m_fontMap()
    , m_fontSize(16)
    , m_dirtyOrthoMatrix(true)
    , m_dirtyViewMatrix(true)
    , m_dirtyProjectionMatrix(true)
    , m_orthoMatrix()
    , m_viewMatrix()
    , m_projectionMatrix()
    , m_rectObject()
    , m_circleObject()
    , m_spriteObject()
    , m_textObject()
    , m_planeObject()
    , m_planeLightingObject()
    , m_boxObject()
    , m_boxLightingObject()
    , m_tileBatchUltraSmall()
    , m_tileBatchSmall()
    , m_tileBatchMedium()
    , m_tileBatchLarge()
    , m_tileBatchUltraLarge()
{
}

void Renderer::position(const Math::Vector3& position)
{
    m_position = position;
    m_dirtyViewMatrix = true;
}

void Renderer::lookAt(const Math::Vector3& lookAt)
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

void Renderer::lightEnable() { m_lightEnable = true; }

void Renderer::lightDisable() { m_lightEnable = false; }

void Renderer::lightDirection(const Math::Vector3& lightDirection) { m_lightDirection = lightDirection; }

void Renderer::textFont(const std::shared_ptr<FontMap>& fontMap) { m_fontMap = fontMap; }

void Renderer::textFontSize(int32_t fontSize) { m_fontSize = fontSize; }

void Renderer::guiBegin()
{
    Engine::getInstance()->getDevice()->getSurface()->guiBegin();
}

void Renderer::guiEnd()
{
    Engine::getInstance()->getDevice()->getSurface()->guiEnd();
}

void Renderer::begin()
{
    Engine::getInstance()->getDevice()->getSurface()->begin();
}

void Renderer::end()
{
    Engine::getInstance()->getDevice()->getSurface()->end();
    Constant::release();
}

void Renderer::drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color)
{
    initRect();
    auto constant = Constant::rent(Constant::Layout::Color);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix::rotateZ(degree),
        Math::Matrix::scale(Math::Vector3(size, 1.0f)));
    constant->setModelMatrix(modelMatrix);
    constant->setViewMatrix(Math::Matrix());
    constant->setProjectionMatrix(getOrthoMatrix());
    constant->setColor(color);
    renderObject(m_rectObject, constant);
}

void Renderer::drawCircle(const Math::Vector2& position, const Math::Vector2& size, const Color& color)
{
    initCircle();
    auto constant = Constant::rent(Constant::Layout::Color);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3(size, 1.0f)));
    constant->setModelMatrix(modelMatrix);
    constant->setViewMatrix(Math::Matrix());
    constant->setProjectionMatrix(getOrthoMatrix());
    constant->setColor(color);
    renderObject(m_circleObject, constant);
}

void Renderer::drawSprite(const Math::Vector2& position, const Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color)
{
    initSprite();
    auto constant = Constant::rent(Constant::Layout::TextureAndColor);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix::rotateZ(degree),
        Math::Matrix::scale(Math::Vector3(size, 1.0f)));
    constant->setModelMatrix(modelMatrix);
    constant->setViewMatrix(Math::Matrix());
    constant->setProjectionMatrix(getOrthoMatrix());
    constant->setColor(color);
    constant->setTexture(texture);
    renderObject(m_spriteObject, constant);
}

void Renderer::drawText(const Math::Vector2& position, TextAlignX alignX, TextAlignY alignY, const std::u16string& label, const Color& color)
{
    if (!m_fontMap) {
        return;
    }
    initText();
    Math::Vector2 offset(position);
    Math::Vector2 size = measureText(label, alignY);
    switch (alignX) {
    case TextAlignX::Center:
        offset.x() -= size.x() / 2.0f;
        break;
    case TextAlignX::Left:
        break;
    case TextAlignX::Right:
        offset.x() -= size.x();
        break;
    }
    switch (alignY) {
    case TextAlignY::Center:
        offset.y() -= size.y() / 2.0f;
        break;
    case TextAlignY::Top:
        offset.y() -= size.y();
        break;
    case TextAlignY::Bottom:
        offset.y() += size.y() / 2.0f;
        break;
    }
    for (char16_t c : label) {
        auto fontSprite = m_fontMap->load(m_fontSize, c);
        float xpos = offset.x() + fontSprite->metrics.bearing.x() + (fontSprite->metrics.size.x() / 2);
        float ypos = offset.y() - (fontSprite->metrics.size.y() - fontSprite->metrics.bearing.y()) + (fontSprite->metrics.size.y() / 2);

        auto constant = Constant::rent(Constant::Layout::TextureAndColor);
        auto modelMatrix = Math::Matrix::transform(
            Math::Matrix::translate(Math::Vector3(Math::Vector2({ xpos, ypos }), 0)),
            Math::Matrix(),
            Math::Matrix::scale(Math::Vector3({ static_cast<float>(fontSprite->metrics.size.x()), static_cast<float>(fontSprite->metrics.size.y()), 1.0f })));
        constant->setModelMatrix(modelMatrix);
        constant->setViewMatrix(Math::Matrix());
        constant->setProjectionMatrix(getOrthoMatrix());
        constant->setColor(color);
        constant->setTexture(fontSprite->texture);
        renderObject(m_textObject, constant);
        offset.x() += fontSprite->metrics.advance.x() >> 6;
    }
}

Math::Vector2 Renderer::measureText(const std::u16string& label, TextAlignY alignY)
{
    auto fontSprites = m_fontMap->load(this->m_fontSize, label);
    Math::Vector2 offset({ 0, 0 });
    float maxY = -1;
    for (auto fontSprite : fontSprites) {
        auto size = fontSprite->metrics.size.y();
        if (alignY == TextAlignY::Bottom) {
            size = (fontSprite->metrics.size.y() - fontSprite->metrics.bearing.y());
        }
        if (maxY < size) {
            maxY = size;
        }
    }
    offset.y() = maxY;
    for (auto fontSprite : fontSprites) {
        offset.x() += fontSprite->metrics.advance.x() >> 6;
    }
    return offset;
}

void Renderer::drawPlane(const Math::Vector3& position, const Math::Vector3& size, const Math::Quaternion& rotation, const Color& color)
{
    if (!m_lightEnable) {
        initPlane();
        auto constant = Constant::rent(Constant::Layout::Color);
        auto modelMatrix = Math::Matrix::transform(
            Math::Matrix::translate(position),
            Math::Quaternion::toMatrix(rotation),
            Math::Matrix::scale(size));
        constant->setModelMatrix(modelMatrix);
        constant->setViewMatrix(getLookAtMatrix());
        constant->setProjectionMatrix(getPerspectiveMatrix());
        constant->setColor(color);
        renderObject(m_planeObject, constant);
    } else {
        initPlaneLighting();
        auto constant = Constant::rent(Constant::Layout::LightColor);
        auto modelMatrix = Math::Matrix::transform(
            Math::Matrix::translate(position),
            Math::Quaternion::toMatrix(rotation),
            Math::Matrix::scale(size));
        constant->setModelMatrix(modelMatrix);
        constant->setViewMatrix(getLookAtMatrix());
        constant->setProjectionMatrix(getPerspectiveMatrix());
        constant->setColor(color);
        constant->setLightDirection(m_lightDirection);
        renderObject(m_planeLightingObject, constant);
    }
}

void Renderer::drawBox(const Math::Vector3& position, const Math::Vector3& size, const Math::Quaternion& rotation, const Color& color)
{
    if (!m_lightEnable) {
        initBox();
        auto constant = Constant::rent(Constant::Layout::Color);
        auto modelMatrix = Math::Matrix::transform(
            Math::Matrix::translate(position),
            Math::Quaternion::toMatrix(rotation),
            Math::Matrix::scale(size));
        constant->setModelMatrix(modelMatrix);
        constant->setViewMatrix(getLookAtMatrix());
        constant->setProjectionMatrix(getPerspectiveMatrix());
        constant->setColor(color);
        renderObject(m_boxObject, constant);
    } else {
        initBoxLighting();
        auto constant = Constant::rent(Constant::Layout::LightColor);
        auto modelMatrix = Math::Matrix::transform(
            Math::Matrix::translate(position),
            Math::Quaternion::toMatrix(rotation),
            Math::Matrix::scale(size));
        constant->setModelMatrix(modelMatrix);
        constant->setViewMatrix(getLookAtMatrix());
        constant->setProjectionMatrix(getPerspectiveMatrix());
        constant->setColor(color);
        constant->setLightDirection(m_lightDirection);
        renderObject(m_boxLightingObject, constant);
    }
}

void Renderer::drawTiles()
{
    if (!m_tileBatchUltraSmall) {
        m_tileBatchUltraSmall = TileBatch::create(TileBufferUltraSmall::create(10));
    }
    if (!m_tileBatchSmall) {
        m_tileBatchSmall = TileBatch::create(TileBufferSmall::create(10));
    }
    if (!m_tileBatchMedium) {
        m_tileBatchMedium = TileBatch::create(TileBufferMedium::create(20));
    }
    if (!m_tileBatchLarge) {
        m_tileBatchLarge = TileBatch::create(TileBufferLarge::create(10));
    }
    if (!m_tileBatchUltraLarge) {
        m_tileBatchUltraLarge = TileBatch::create(TileBufferUltraLarge::create(10));
    }
    Engine::getInstance()->getDevice()->getSurface()->render(m_tileBatchUltraSmall);
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
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }

        Output vsMain(float2 pos : POSITION) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 0, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
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
    std::vector<Math::Vector2> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(Math::Vector2({ left, top }));
    vertices.push_back(Math::Vector2({ right, top }));
    vertices.push_back(Math::Vector2({ right, bottom }));
    vertices.push_back(Math::Vector2({ left, bottom }));
    m_rectObject.vertexBuffer->allocate(sizeof(Math::Vector2) * vertices.size());
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
    m_rectObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 2, false, false);
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
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }

        Output vsMain(float2 pos : POSITION) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 0, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
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
    std::vector<Math::Vector2> vertices;
    std::vector<uint32_t> indices;
    float degree = 0.0f;
    vertices.emplace_back(Math::Vector2({ 0.0f, 0.0f }));
    while (degree < 360.0f) {
        float x = Math::Mathf::cos(Math::Mathf::Deg2Rad * degree) * 0.5f;
        float y = Math::Mathf::sin(Math::Mathf::Deg2Rad * degree) * 0.5f;
        vertices.emplace_back(Math::Vector2({ x, y }));
        degree += 5.0f;
    }
    m_circleObject.vertexBuffer->allocate(sizeof(Math::Vector2) * vertices.size());
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
    m_circleObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 2, false, false);
}

void Renderer::initSprite()
{
    if (m_spriteObject.pso != nullptr) {
        return;
    }
    auto shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 uv : TEXCOORD;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }

        Output vsMain(float2 pos : POSITION, float2 uv : TEXCOORD) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 0, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
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
    m_spriteObject.vertexBuffer = Buffer::create();
    m_spriteObject.indexBuffer = Buffer::create();
    std::vector<VertexTexCoord2D> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
    m_spriteObject.vertexBuffer->allocate(sizeof(VertexTexCoord2D) * vertices.size());
    m_spriteObject.vertexBuffer->update(vertices.data());
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(1);
    m_spriteObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_spriteObject.indexBuffer->update(indices.data());
    m_spriteObject.indexLength = indices.size();
    m_spriteObject.pso = PipelineStateObject::create(shader, Constant::Layout::TextureAndColor, PrimitiveType::Triangles, 2, false, true);
}

void Renderer::initText()
{
    if (m_textObject.pso != nullptr) {
        return;
    }
    auto shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 uv : TEXCOORD;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }

        Output vsMain(float2 pos : POSITION, float2 uv : TEXCOORD) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 0, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
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
            float4 col = float4(tex.Sample(smp, input.uv));
            col.w = col.x * input.color.w;
            col.x = input.color.x;
            col.y = input.color.y;
            col.z = input.color.z;
            return col;
        })",
        "psMain");
    m_textObject.vertexBuffer = Buffer::create();
    m_textObject.indexBuffer = Buffer::create();
    std::vector<VertexTexCoord2D> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
    m_textObject.vertexBuffer->allocate(sizeof(VertexTexCoord2D) * vertices.size());
    m_textObject.vertexBuffer->update(vertices.data());
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(1);
    m_textObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_textObject.indexBuffer->update(indices.data());
    m_textObject.indexLength = indices.size();
    m_textObject.pso = PipelineStateObject::create(shader, Constant::Layout::TextureAndColor, PrimitiveType::Triangles, 2, false, true);
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
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }

        Output vsMain(float3 pos : POSITION) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
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
    std::vector<Math::Vector3> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.emplace_back(Math::Vector3({ left, bottom, 0 }));
    vertices.emplace_back(Math::Vector3({ left, top, 0 }));
    vertices.emplace_back(Math::Vector3({ right, bottom, 0 }));
    vertices.emplace_back(Math::Vector3({ right, top, 0 }));
    m_planeObject.vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
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
    m_planeObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 3, false, false);
}

void Renderer::initPlaneLighting()
{
    if (m_planeLightingObject.pso != nullptr) {
        return;
    }
    auto shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }
        cbuffer cbuff4 : register(b4) { float3 lightDirection; }

        Output vsMain(float3 pos : POSITION, float3 normal : NORMAL) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);

            float3 nor = normalize(mul(modelMatrix, float4(normal, 1)).xyz);
            float col = saturate(dot(nor, lightDirection));
            col = col * 0.5f + 0.5f;

            output.color = float4(color.x*col, color.y*col, color.z*col, color.w);
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
    m_planeLightingObject.vertexBuffer = Buffer::create();
    m_planeLightingObject.indexBuffer = Buffer::create();
    std::vector<VertexNormal3D> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, 0 }), Math::Vector3({ 0, 0, -1 })));
    m_planeLightingObject.vertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
    m_planeLightingObject.vertexBuffer->update(vertices.data());
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
    m_planeLightingObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_planeLightingObject.indexBuffer->update(indices.data());
    m_planeLightingObject.indexLength = indices.size();
    m_planeLightingObject.pso = PipelineStateObject::create(shader, Constant::Layout::LightColor, PrimitiveType::Triangles, 3, true, false);
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
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }

        Output vsMain(float3 pos : POSITION) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
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
    std::vector<Math::Vector3> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    const float forward = 0.5f;
    const float backward = -0.5f;
    vertices.emplace_back(Math::Vector3({ left, bottom, backward }));
    vertices.emplace_back(Math::Vector3({ left, top, backward }));
    vertices.emplace_back(Math::Vector3({ right, bottom, backward }));
    vertices.emplace_back(Math::Vector3({ right, top, backward }));
    vertices.emplace_back(Math::Vector3({ left, bottom, forward }));
    vertices.emplace_back(Math::Vector3({ left, top, forward }));
    vertices.emplace_back(Math::Vector3({ right, bottom, forward }));
    vertices.emplace_back(Math::Vector3({ right, top, forward }));
    m_boxObject.vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
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
    m_boxObject.pso = PipelineStateObject::create(shader, Constant::Layout::Color, PrimitiveType::Triangles, 3, false, false);
}

void Renderer::initBoxLighting()
{
    if (m_boxLightingObject.pso != nullptr) {
        return;
    }
    auto shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
        cbuffer cbuff3 : register(b3) { float4 color; }
        cbuffer cbuff4 : register(b4) { float3 lightDirection; }

        Output vsMain(float3 pos : POSITION, float3 normal : NORMAL) {
            Output output;
            
            output.svpos = mul(modelMatrix, float4(pos, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);

            float3 nor = normalize(mul(modelMatrix, float4(normal, 1)).xyz);
            float col = saturate(dot(nor, lightDirection));
            col = col * 0.5f + 0.5f;

            output.color = float4(color.x * col, color.y * col, color.z * col, color.w);
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
    m_boxLightingObject.vertexBuffer = Buffer::create();
    m_boxLightingObject.indexBuffer = Buffer::create();
    std::vector<VertexNormal3D> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    const float forward = 0.5f;
    const float backward = -0.5f;

    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, backward }), Math::Vector3({ 0, 0, -1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, backward }), Math::Vector3({ 0, 0, -1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, backward }), Math::Vector3({ 0, 0, -1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, backward }), Math::Vector3({ 0, 0, -1 })));

    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, forward }), Math::Vector3({ 0, 0, 1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, forward }), Math::Vector3({ 0, 0, 1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, forward }), Math::Vector3({ 0, 0, 1 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, forward }), Math::Vector3({ 0, 0, 1 })));

    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, forward }), Math::Vector3({ -1, 0, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, backward }), Math::Vector3({ -1, 0, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, forward }), Math::Vector3({ -1, 0, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, backward }), Math::Vector3({ -1, 0, 0 })));

    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, forward }), Math::Vector3({ 1, 0, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, forward }), Math::Vector3({ 1, 0, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, backward }), Math::Vector3({ 1, 0, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, backward }), Math::Vector3({ 1, 0, 0 })));

    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, forward }), Math::Vector3({ 0, 1, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, forward }), Math::Vector3({ 0, 1, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, top, backward }), Math::Vector3({ 0, 1, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, top, backward }), Math::Vector3({ 0, 1, 0 })));

    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, forward }), Math::Vector3({ 0, -1, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ left, bottom, backward }), Math::Vector3({ 0, -1, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, forward }), Math::Vector3({ 0, -1, 0 })));
    vertices.emplace_back(VertexNormal3D(Math::Vector3({ right, bottom, backward }), Math::Vector3({ 0, -1, 0 })));
    m_boxLightingObject.vertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
    m_boxLightingObject.vertexBuffer->update(vertices.data());
    // backward
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(3);
    indices.emplace_back(2);
    indices.emplace_back(1);
    // forward
    indices.emplace_back(4);
    indices.emplace_back(5);
    indices.emplace_back(6);
    indices.emplace_back(7);
    indices.emplace_back(6);
    indices.emplace_back(5);
    // left
    indices.emplace_back(8);
    indices.emplace_back(9);
    indices.emplace_back(10);
    indices.emplace_back(11);
    indices.emplace_back(10);
    indices.emplace_back(9);
    // right
    indices.emplace_back(12);
    indices.emplace_back(13);
    indices.emplace_back(14);
    indices.emplace_back(15);
    indices.emplace_back(14);
    indices.emplace_back(13);
    // top
    indices.emplace_back(16);
    indices.emplace_back(17);
    indices.emplace_back(18);
    indices.emplace_back(19);
    indices.emplace_back(18);
    indices.emplace_back(17);
    // bottom
    indices.emplace_back(20);
    indices.emplace_back(21);
    indices.emplace_back(22);
    indices.emplace_back(23);
    indices.emplace_back(22);
    indices.emplace_back(21);
    m_boxLightingObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_boxLightingObject.indexBuffer->update(indices.data());
    m_boxLightingObject.indexLength = indices.size();
    m_boxLightingObject.pso = PipelineStateObject::create(shader, Constant::Layout::LightColor, PrimitiveType::Triangles, 3, true, false);
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

Math::Matrix Renderer::getOrthoMatrix()
{
    if (m_dirtyOrthoMatrix) {
        m_dirtyOrthoMatrix = false;
        m_orthoMatrix = Math::Matrix::ortho(
            static_cast<float>(Screen::getWidth()),
            static_cast<float>(Screen::getHeight()));
    }
    return m_orthoMatrix;
}

Math::Matrix Renderer::getLookAtMatrix()
{
    if (m_dirtyViewMatrix) {
        m_dirtyViewMatrix = false;
        m_viewMatrix = Math::Matrix::lookAt(m_position, m_lookAt, Math::Vector3({ 0, 1, 0 }));
    }
    return m_viewMatrix;
}

Math::Matrix Renderer::getPerspectiveMatrix()
{
    if (m_dirtyProjectionMatrix) {
        m_dirtyProjectionMatrix = false;
        m_projectionMatrix = Math::Matrix::perspective(m_fovY,
            static_cast<float>(Screen::getWidth()) / static_cast<float>(Screen::getHeight()),
            m_zNear,
            m_zFar);
    }
    return m_projectionMatrix;
}
}