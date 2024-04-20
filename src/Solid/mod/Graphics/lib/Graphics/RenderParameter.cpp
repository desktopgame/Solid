#include <Graphics/RenderParameter.hpp>

namespace Lib::Graphics {
RenderParameter::RenderParameter()
    : m_transform()
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

bool RenderParameter::useTexture() const
{
    return false;
}

void RenderParameter::setColor(const Math::Vector4& color)
{
    m_color = color;
    m_useColor = true;
}
Math::Vector4 RenderParameter::getColor() const { return m_color; }
bool RenderParameter::useColor() const { return m_useColor; }
}