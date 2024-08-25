#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/RenderParameter.hpp>
#include <Graphics/Surface.hpp>

namespace Lib::Graphics {
// public
std::shared_ptr<RenderContext> RenderContext::create(PrimitiveType primitiveType)
{
    auto renderContext = std::shared_ptr<RenderContext>(new RenderContext(primitiveType));
    return renderContext;
}

RenderContext::~RenderContext()
{
}

void RenderContext::updateVertex(const Math::Vector2* data, int32_t len)
{
    m_vertexBuffer->allocate(sizeof(Math::Vector2) * len);
    m_vertexBuffer->update(data);
    m_vertexComponent = 2;
    m_vertexLength = len;
    m_isUsingTexCoord = false;
}

void RenderContext::updateVertex(const VertexData2D* data, int32_t len)
{
    m_vertexBuffer->allocate(sizeof(VertexData2D) * len);
    m_vertexBuffer->update(data);
    m_vertexComponent = 2;
    m_vertexLength = len;
    m_isUsingTexCoord = true;
}
void RenderContext::updateVertex(const Math::Vector3* data, int32_t len)
{
    m_vertexBuffer->allocate(sizeof(Math::Vector3) * len);
    m_vertexBuffer->update(data);
    m_vertexComponent = 3;
    m_vertexLength = len;
    m_isUsingTexCoord = false;
}
void RenderContext::updateVertex(const VertexData3D* data, int32_t len)
{
    m_vertexBuffer->allocate(sizeof(VertexData3D) * len);
    m_vertexBuffer->update(data);
    m_vertexComponent = 3;
    m_vertexLength = len;
    m_isUsingTexCoord = true;
}
void RenderContext::updateIndex(const uint32_t* data, int32_t len)
{
    m_indexBuffer->allocate(sizeof(uint32_t) * len);
    m_indexBuffer->update(data);
    m_indexLength = len;
}

PrimitiveType RenderContext::getPrimitiveType() const { return m_primitiveType; }
int32_t RenderContext::getVertexComponent() const { return m_vertexComponent; }
int32_t RenderContext::getIndexLength() const { return m_indexLength; }
bool RenderContext::isUsingTexCoord() const { return m_isUsingTexCoord; }
const std::shared_ptr<Buffer>& RenderContext::getVertexBuffer() const { return m_vertexBuffer; }
const std::shared_ptr<Buffer>& RenderContext::getIndexBuffer() const { return m_indexBuffer; }
// private
RenderContext::RenderContext(PrimitiveType primitiveType)
    : m_primitiveType(primitiveType)
    , m_vertexComponent(0)
    , m_vertexLength(0)
    , m_indexLength(0)
    , m_isUsingTexCoord(false)
    , m_vertexBuffer(Buffer::create())
    , m_indexBuffer(Buffer::create())
{
}
}