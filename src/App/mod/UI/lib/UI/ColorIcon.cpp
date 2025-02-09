#include <UI/ColorIcon.hpp>

namespace Lib::UI {

ColorIcon::ColorIcon()
    : Component()
{
}

void ColorIcon::update() { }
void ColorIcon::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getGlobalPosition(), getSize(), 0.0f, getForegroundColor());
}
}