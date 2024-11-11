#include <Graphics/Buffer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/FontSprite.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/TileBatch.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/UniformPool.hpp>
#include <Graphics/VertexNormal3D.hpp>
#include <Graphics/VertexNormalTexCoord3D.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Math/Mathf.hpp>

namespace Lib::Graphics {
// public
Renderer::Renderer()
    : m_fontMap()
    , m_fontSize(16)
    , m_rectObject()
    , m_circleObject()
    , m_spriteObject()
    , m_textObject()
{
}

void Renderer::textFont(const std::shared_ptr<FontMap>& fontMap) { m_fontMap = fontMap; }

void Renderer::textFontSize(int32_t fontSize) { m_fontSize = fontSize; }

void Renderer::drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color)
{
    initRect();
    auto ub = UniformPool::rent(Metadata::ProgramTable::Color2D);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix::rotateZ(degree),
        Math::Matrix::scale(Math::Vector3(size, 1.0f)));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Math::Matrix();
    uCamera.projectionMatrix = Camera::getOrthoMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);

    renderObject(m_rectObject, ub);
}

void Renderer::drawCircle(const Math::Vector2& position, const Math::Vector2& size, const Color& color)
{
    initCircle();
    auto ub = UniformPool::rent(Metadata::ProgramTable::Color2D);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3(size, 1.0f)));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Math::Matrix();
    uCamera.projectionMatrix = Camera::getOrthoMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);
    renderObject(m_circleObject, ub);
}

void Renderer::drawSprite(const Math::Vector2& position, const Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color)
{
    initSprite();
    auto ub = UniformPool::rent(Metadata::ProgramTable::Texture2D);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3(size, 1.0f)));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Math::Matrix();
    uCamera.projectionMatrix = Camera::getOrthoMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);
    ub->setPS(0, texture);
    renderObject(m_spriteObject, ub);
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

        auto ub = UniformPool::rent(Metadata::Text2D);
        auto modelMatrix = Math::Matrix::transform(
            Math::Matrix::translate(Math::Vector3(Math::Vector2({ xpos, ypos }), 0)),
            Math::Matrix(),
            Math::Matrix::scale(Math::Vector3({ static_cast<float>(fontSprite->metrics.size.x()), static_cast<float>(fontSprite->metrics.size.y()), 1.0f })));
        Reflect::UCamera uCamera;
        uCamera.modelMatrix = modelMatrix;
        uCamera.viewMatrix = Math::Matrix();
        uCamera.projectionMatrix = Camera::getOrthoMatrix();
        ub->setVS(0, &uCamera);

        Reflect::UVector4 uColor;
        uColor.value = color;
        ub->setVS(1, &uColor);
        ub->setPS(0, fontSprite->texture);
        renderObject(m_textObject, ub);
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

void Renderer::drawPlane(const Math::Vector3& position, float scale, const Math::Quaternion& rotation, const Math::Vector4& color)
{
    initPlane();
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshColor3D);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3({ scale, scale, 1.0f })));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);
    renderObject(m_planeObject, ub);
}

void Renderer::drawPlaneTexture(const Math::Vector3& position, float scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color)
{
    initPlaneTexture();
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshTexture3D);
    auto modelMatrix = Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3({ scale, scale, 1.0f })));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);

    ub->setPS(0, texture);
    renderObject(m_planeTextureObject, ub);
}
// private
void Renderer::initRect()
{
    if (m_rectObject.rc != nullptr) {
        return;
    }
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
    m_rectObject.rc = RenderContext::get(Metadata::ProgramTable::Color2D);
}

void Renderer::initCircle()
{
    if (m_circleObject.rc != nullptr) {
        return;
    }
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
    m_circleObject.rc = RenderContext::get(Metadata::ProgramTable::Color2D);
}

void Renderer::initSprite()
{
    if (m_spriteObject.rc != nullptr) {
        return;
    }
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
    m_spriteObject.rc = RenderContext::get(Metadata::ProgramTable::Texture2D);
}

void Renderer::initText()
{
    if (m_textObject.rc != nullptr) {
        return;
    }
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
    m_textObject.rc = RenderContext::get(Metadata::ProgramTable::Text2D);
}

void Renderer::initPlane()
{
    if (m_planeObject.rc != nullptr) {
        return;
    }
    m_planeObject.vertexBuffer = Buffer::create();
    m_planeObject.indexBuffer = Buffer::create();
    std::vector<VertexNormal3D> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexNormal3D(Math::Vector3({ left, bottom, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ left, top, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ right, bottom, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ right, top, 0 }), Math::Vector3({ 0, 0, -1 })));
    m_planeObject.vertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
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
    m_planeObject.rc = RenderContext::get(Metadata::ProgramTable::MeshColor3D);
}

void Renderer::initPlaneTexture()
{
    if (m_planeTextureObject.rc != nullptr) {
        return;
    }
    m_planeTextureObject.vertexBuffer = Buffer::create();
    m_planeTextureObject.indexBuffer = Buffer::create();
    std::vector<VertexNormalTexCoord3D> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ left, bottom, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ left, top, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ right, bottom, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ right, top, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 1, 0 })));
    m_planeTextureObject.vertexBuffer->allocate(sizeof(VertexNormalTexCoord3D) * vertices.size());
    m_planeTextureObject.vertexBuffer->update(vertices.data());
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
    m_planeTextureObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_planeTextureObject.indexBuffer->update(indices.data());
    m_planeTextureObject.indexLength = indices.size();
    m_planeTextureObject.rc = RenderContext::get(Metadata::ProgramTable::MeshTexture3D);
}

void Renderer::renderObject(const Object& object, const std::shared_ptr<UniformBuffer>& ub)
{
    Engine::getInstance()->getDevice()->getSurface()->render(
        object.rc,
        ub,
        object.vertexBuffer,
        object.indexBuffer,
        object.indexLength);
}
}
