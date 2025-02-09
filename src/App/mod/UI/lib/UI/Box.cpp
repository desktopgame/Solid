#include <UI/Box.hpp>
#include <UI/BoxLayout.hpp>
#include <UI/Panel.hpp>

namespace Lib::UI {
std::shared_ptr<Container> Box::createHorizontalBox()
{
    auto panel = std::make_shared<Panel>();
    panel->setLayout(std::make_shared<BoxLayout>(BoxLayout::Orientation::Horizontal));
    return panel;
}

std::shared_ptr<Container> Box::createVerticalBox()
{
    auto panel = std::make_shared<Panel>();
    panel->setLayout(std::make_shared<BoxLayout>(BoxLayout::Orientation::Vertical));
    return panel;
}
}