#include <Graphics/RenderParameter.hpp>

namespace Lib::Graphics {
RenderParameter::RenderParameter()
    : m_transform()
    , m_texture()
    , m_color()
    , m_useColor()
{
}

void RenderParameter::setTransform(const Math::Matrix& transform)
{
    m_transform = transform;
}
Math::Matrix RenderParameter::getTransform() const
{
    return m_transform;
}

void RenderParameter::setTexture(const std::shared_ptr<Texture>& texture) { m_texture = texture; }
std::shared_ptr<Texture> RenderParameter::getTexture() const { return m_texture; }
bool RenderParameter::useTexture() const
{
    return m_texture != nullptr;
}

void RenderParameter::setColor(const Math::Vector4& color)
{
    m_color = color;
    m_useColor = true;
}
Math::Vector4 RenderParameter::getColor() const { return m_color; }
bool RenderParameter::useColor() const { return m_useColor; }
}