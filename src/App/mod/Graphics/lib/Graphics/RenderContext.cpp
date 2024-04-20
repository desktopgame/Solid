#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/RenderParameter.hpp>

namespace Lib::Graphics {
// public
std::shared_ptr<RenderContext> RenderContext::create()
{
    auto renderContext = std::shared_ptr<RenderContext>(new RenderContext());
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
}

void RenderContext::updateVertex(const VertexData2D* data, int32_t len)
{
    m_vertexBuffer->allocate(sizeof(VertexData2D) * len);
    m_vertexBuffer->update(data);
    m_vertexComponent = 2;
    m_vertexLength = len;
}
void RenderContext::updateVertex(const Math::Vector3* data, int32_t len)
{
    m_vertexBuffer->allocate(sizeof(Math::Vector3) * len);
    m_vertexBuffer->update(data);
    m_vertexComponent = 3;
    m_vertexLength = len;
}
void RenderContext::updateIndex(uint32_t* data, int32_t len)
{
    m_indexBuffer->allocate(sizeof(uint32_t) * len);
    m_indexBuffer->update(data);
    m_indexLength = len;
}

int32_t RenderContext::getVertexComponent() const
{
    return m_vertexComponent;
}

uint64_t RenderContext::getVertexVirtualAddress() const
{
    return m_vertexBuffer->getVirtualAddress();
}
int32_t RenderContext::getVertexLength() const
{
    return m_vertexLength;
}
uint64_t RenderContext::getIndexVirtualAddress() const
{
    return m_indexBuffer->getVirtualAddress();
}
int32_t RenderContext::getIndexLength() const
{
    return m_indexLength;
}

bool RenderContext::isUsingTexCoord() const
{
    return m_isUsingTexCoord;
}
// private
RenderContext::RenderContext()
    : m_parameter(std::make_shared<RenderParameter>())
    , m_vertexComponent(0)
    , m_vertexLength(0)
    , m_indexLength(0)
    , m_isUsingTexCoord(false)
    , m_vertexBuffer(Buffer::create())
    , m_indexBuffer(Buffer::create())
{
}
}