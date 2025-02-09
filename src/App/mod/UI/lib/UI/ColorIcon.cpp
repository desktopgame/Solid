#include <UI/ColorIcon.hpp>

namespace Lib::UI {

ColorIcon::ColorIcon()
    : Component()
    , m_color()
{
}

void ColorIcon::update() { }
void ColorIcon::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getGlobalPosition(), getSize(), 0.0f, m_color);
}

void ColorIcon::setColor(const Graphics::Color& color) { m_color = color; }
Graphics::Color ColorIcon::getColor() const { return m_color; }
}