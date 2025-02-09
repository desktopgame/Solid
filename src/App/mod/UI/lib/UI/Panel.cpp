#include <UI/Panel.hpp>

namespace Lib::UI {

Panel::Panel()
    : Container()
{
}
Panel::~Panel() { }

void Panel::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getGlobalPosition(), getSize(), 0.0f, getBackgroundColor());
    Container::draw2D(renderer);
}
}