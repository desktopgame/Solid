#include <Graphics/Buffer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/FontSprite.hpp>
#include <Graphics/Polygon.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/UniformPool.hpp>
#include <Graphics/VertexNormal3D.hpp>
#include <Graphics/VertexNormalTexCoord3D.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Math/Mathf.hpp>

namespace Lib::Graphics {
// public
Renderer::Renderer()
    : m_matrixStack()
    , m_fontMap()
    , m_fontSize(16)
    , m_rectObject()
    , m_circleObject()
    , m_spriteObject()
    , m_textObject()
    , m_planeObject()
    , m_planeWireframeObject()
    , m_planeLineObject()
    , m_planeTextureObject()
    , m_boxObject()
    , m_boxWireframeObject()
    , m_boxLineObject()
    , m_boxTextureObject()
{
}

void Renderer::pushMatrix(const Math::Matrix& m) { m_matrixStack.push(m); }

void Renderer::popMatrix() { m_matrixStack.pop(); }

void Renderer::textFont(const std::shared_ptr<FontMap>& fontMap) { m_fontMap = fontMap; }

void Renderer::textFontSize(int32_t fontSize) { m_fontSize = fontSize; }

void Renderer::drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color)
{
    initRect();
    auto ub = UniformPool::rent(Metadata::ProgramTable::Color2D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix::rotateZ(degree),
        Math::Matrix::scale(Math::Vector3(size, 1.0f))));
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
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3(size, 1.0f))));
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
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(Math::Vector3(position, 0)),
        Math::Matrix(),
        Math::Matrix::scale(Math::Vector3(size, 1.0f))));
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
        auto modelMatrix = applyMatrix(Math::Matrix::transform(
            Math::Matrix::translate(Math::Vector3(Math::Vector2({ xpos, ypos }), 0)),
            Math::Matrix(),
            Math::Matrix::scale(Math::Vector3({ static_cast<float>(fontSprite->metrics.size.x()), static_cast<float>(fontSprite->metrics.size.y()), 1.0f }))));
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

void Renderer::drawPlane(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const Math::Vector4& color, bool isWireframe)
{
    Object& obj = isWireframe ? m_planeWireframeObject : m_planeObject;
    initPlane(obj, isWireframe);
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshColor3D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Quaternion::toMatrix(rotation),
        Math::Matrix::scale(Math::Vector3(scale, 1.0f))));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);
    renderObject(obj, ub);
}

void Renderer::drawPlaneLine(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const Math::Vector4& color, float lineWidth)
{
    initPlaneLine();
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshLine3D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Quaternion::toMatrix(rotation),
        Math::Matrix::scale(Math::Vector3(scale, 1.0f))));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);

    ub->setGS(0, &uCamera);

    Reflect::UFloat uLineWidth;
    uLineWidth.value = lineWidth;
    ub->setGS(1, &uLineWidth);

    Reflect::UVector3 uCameraPosition;
    uCameraPosition.value = Camera::getPosition();
    ub->setGS(2, &uCameraPosition);
    renderObject(m_planeLineObject, ub);
}

void Renderer::drawPlaneTexture(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color)
{
    initPlaneTexture();
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshTexture3D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Quaternion::toMatrix(rotation),
        Math::Matrix::scale(Math::Vector3(scale, 1.0f))));
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

void Renderer::drawBox(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const Math::Vector4& color, bool isWireframe)
{
    Object& obj = isWireframe ? m_boxWireframeObject : m_boxObject;
    initBox(obj, isWireframe);
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshColor3D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Quaternion::toMatrix(rotation),
        Math::Matrix::scale(scale)));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);
    renderObject(obj, ub);
}

void Renderer::drawBoxLine(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const Math::Vector4& color, float lineWidth)
{
    initBoxLine();
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshLine3D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Quaternion::toMatrix(rotation),
        Math::Matrix::scale(scale)));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);

    ub->setGS(0, &uCamera);

    Reflect::UFloat uLineWidth;
    uLineWidth.value = lineWidth;
    ub->setGS(1, &uLineWidth);

    Reflect::UVector3 uCameraPosition;
    uCameraPosition.value = Camera::getPosition();
    ub->setGS(2, &uCameraPosition);
    renderObject(m_boxLineObject, ub);
}

void Renderer::drawBoxTexture(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color)
{
    initBoxTexture();
    auto ub = UniformPool::rent(Metadata::ProgramTable::MeshTexture3D);
    auto modelMatrix = applyMatrix(Math::Matrix::transform(
        Math::Matrix::translate(position),
        Math::Quaternion::toMatrix(rotation),
        Math::Matrix::scale(scale)));
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = modelMatrix;
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);

    Reflect::UVector4 uColor;
    uColor.value = color;
    ub->setVS(1, &uColor);

    ub->setPS(0, texture);
    renderObject(m_boxTextureObject, ub);
}
// private
void Renderer::initRect()
{
    if (m_rectObject.rc != nullptr) {
        return;
    }
    m_rectObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_rectObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<Math::Vector2> vertices;
    std::vector<uint32_t> indices;
    Polygon::generateRect(vertices, indices);
    m_rectObject.vertexBuffer->allocate(sizeof(Math::Vector2) * vertices.size());
    m_rectObject.vertexBuffer->update(vertices.data());
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
    m_circleObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_circleObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<Math::Vector2> vertices;
    std::vector<uint32_t> indices;
    Polygon::generateCircle(vertices, indices);
    m_circleObject.vertexBuffer->allocate(sizeof(Math::Vector2) * vertices.size());
    m_circleObject.vertexBuffer->update(vertices.data());
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
    m_spriteObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_spriteObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<VertexTexCoord2D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generateRect(vertices, indices);
    m_spriteObject.vertexBuffer->allocate(sizeof(VertexTexCoord2D) * vertices.size());
    m_spriteObject.vertexBuffer->update(vertices.data());
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
    m_textObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_textObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<VertexTexCoord2D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generateRect(vertices, indices);
    m_textObject.vertexBuffer->allocate(sizeof(VertexTexCoord2D) * vertices.size());
    m_textObject.vertexBuffer->update(vertices.data());
    m_textObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_textObject.indexBuffer->update(indices.data());
    m_textObject.indexLength = indices.size();
    m_textObject.rc = RenderContext::get(Metadata::ProgramTable::Text2D);
}

void Renderer::initPlane(Object& dst, bool isWireframe)
{
    if (dst.rc != nullptr) {
        return;
    }
    dst.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    dst.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<uint32_t> indices;
    if (isWireframe) {
        std::vector<Math::Vector3> vertices;
        Polygon::generatePlane(vertices, indices);
        dst.vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
        dst.vertexBuffer->update(vertices.data());
    } else {
        std::vector<VertexNormal3D> vertices;
        Polygon::generatePlane(vertices, indices);
        dst.vertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
        dst.vertexBuffer->update(vertices.data());
    }
    dst.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    dst.indexBuffer->update(indices.data());
    dst.indexLength = indices.size();

    if (isWireframe) {
        dst.rc = RenderContext::get(Metadata::ProgramTable::MeshWireframe3D);
    } else {
        dst.rc = RenderContext::get(Metadata::ProgramTable::MeshColor3D);
    }
}

void Renderer::initPlaneLine()
{
    if (m_planeLineObject.rc != nullptr) {
        return;
    }
    m_planeLineObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_planeLineObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<uint32_t> indices;
    std::vector<Math::Vector3> vertices;
    Polygon::generatePlane(vertices, indices);
    m_planeLineObject.vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
    m_planeLineObject.vertexBuffer->update(vertices.data());
    m_planeLineObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_planeLineObject.indexBuffer->update(indices.data());
    m_planeLineObject.indexLength = indices.size();
    m_planeLineObject.rc = RenderContext::get(Metadata::ProgramTable::MeshLine3D);
}

void Renderer::initPlaneTexture()
{
    if (m_planeTextureObject.rc != nullptr) {
        return;
    }
    m_planeTextureObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_planeTextureObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<VertexNormalTexCoord3D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generatePlane(vertices, indices);
    m_planeTextureObject.vertexBuffer->allocate(sizeof(VertexNormalTexCoord3D) * vertices.size());
    m_planeTextureObject.vertexBuffer->update(vertices.data());
    m_planeTextureObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_planeTextureObject.indexBuffer->update(indices.data());
    m_planeTextureObject.indexLength = indices.size();
    m_planeTextureObject.rc = RenderContext::get(Metadata::ProgramTable::MeshTexture3D);
}

void Renderer::initBox(Object& dst, bool isWireframe)
{
    if (dst.rc != nullptr) {
        return;
    }
    dst.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    dst.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<uint32_t> indices;
    if (isWireframe) {
        std::vector<Math::Vector3> vertices;
        Polygon::generateBox(vertices, indices);
        dst.vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
        dst.vertexBuffer->update(vertices.data());
    } else {
        std::vector<VertexNormal3D> vertices;
        Polygon::generateBox(vertices, indices);
        dst.vertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
        dst.vertexBuffer->update(vertices.data());
    }
    dst.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    dst.indexBuffer->update(indices.data());
    dst.indexLength = indices.size();
    if (isWireframe) {
        dst.rc = RenderContext::get(Metadata::ProgramTable::MeshWireframe3D);
    } else {
        dst.rc = RenderContext::get(Metadata::ProgramTable::MeshColor3D);
    }
}

void Renderer::initBoxLine()
{
    if (m_boxLineObject.rc != nullptr) {
        return;
    }
    m_boxLineObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_boxLineObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<uint32_t> indices;
    std::vector<Math::Vector3> vertices;
    Polygon::generateBox(vertices, indices);
    m_boxLineObject.vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
    m_boxLineObject.vertexBuffer->update(vertices.data());
    m_boxLineObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_boxLineObject.indexBuffer->update(indices.data());
    m_boxLineObject.indexLength = indices.size();
    m_boxLineObject.rc = RenderContext::get(Metadata::ProgramTable::MeshLine3D);
}

void Renderer::initBoxTexture()
{
    if (m_boxTextureObject.rc != nullptr) {
        return;
    }
    m_boxTextureObject.vertexBuffer = Buffer::create(Buffer::Type::Vertex);
    m_boxTextureObject.indexBuffer = Buffer::create(Buffer::Type::Vertex);
    std::vector<VertexNormalTexCoord3D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generateBox(vertices, indices);
    m_boxTextureObject.vertexBuffer->allocate(sizeof(VertexNormalTexCoord3D) * vertices.size());
    m_boxTextureObject.vertexBuffer->update(vertices.data());
    m_boxTextureObject.indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_boxTextureObject.indexBuffer->update(indices.data());
    m_boxTextureObject.indexLength = indices.size();
    m_boxTextureObject.rc = RenderContext::get(Metadata::ProgramTable::MeshTexture3D);
}

Math::Matrix Renderer::applyMatrix(const Math::Matrix& m) const
{
    return m * m_matrixStack.mult();
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
