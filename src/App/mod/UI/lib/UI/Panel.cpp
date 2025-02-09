#include <UI/Panel.hpp>

namespace Lib::UI {

Panel::Panel()
    : Container()
{
}
Panel::~Panel() { }

void Panel::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getPosition(), getSize(), 0.0f, Graphics::Color({ 0.2f, 0.2f, 0.2f, 1.0f }));
    Container::draw2D(renderer);
}
}