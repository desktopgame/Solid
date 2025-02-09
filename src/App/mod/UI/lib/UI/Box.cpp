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

std::shared_ptr<Container> Box::createHorizontalGlue()
{
    auto panel = std::make_shared<Panel>();
    panel->setMinimumSize(Math::Vector2({ 0, 0 }));
    panel->setPreferredSize(Math::Vector2({ 0, 0 }));
    panel->setMaximumSize(Math::Vector2({ 9999, 0 }));
    panel->setFlexible(true);
    return panel;
}

std::shared_ptr<Container> Box::createVerticalGlue()
{
    auto panel = std::make_shared<Panel>();
    panel->setMinimumSize(Math::Vector2({ 0, 0 }));
    panel->setPreferredSize(Math::Vector2({ 0, 0 }));
    panel->setMaximumSize(Math::Vector2({ 0, 9999 }));
    panel->setFlexible(true);
    return panel;
}
}