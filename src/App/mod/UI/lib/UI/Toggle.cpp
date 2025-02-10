#include <Graphics/Screen.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Mouse.hpp>
#include <UI/Toggle.hpp>

namespace Lib::UI {

Toggle::Toggle()
    : Component()
    , m_isSelected(false)
{
    setMinimumSize(Math::Vector2({ k_width, k_height }));
    setPreferredSize(Math::Vector2({ k_width, k_height }));
}

void Toggle::update()
{
    auto inputSystem = Input::InputSystem::getInstance();
    auto mousePos = inputSystem->getMouse()->getPosition();
    auto mouseStatus = inputSystem->getMouse()->getState(Input::Mouse::Button::Left);

    auto screenPos = (((Math::Vector2)mousePos / (Math::Vector2)Graphics::Screen::getSize()) - Math::Vector2({ 0.5f, 0.5f })) * (Math::Vector2)Graphics::Screen::getSize();
    screenPos.y() *= -1;

    if (isContains(screenPos)) {
        if (mouseStatus == Input::ButtonState::Trigger) {
            bool updated = !isSelected();
            setSelected(updated);
        }
    }
}
void Toggle::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    auto size = Math::Vector2({ k_width, k_height });
    renderer->drawRect(center, size, 0.0f, Graphics::Color({ 0, 0, 0, 1 }));
    renderer->drawRect(center, size - Math::Vector2({ 2, 2 }), 0.0f, getBackgroundColor());

    if (isSelected()) {
        renderer->drawRect(center + Math::Vector2({ -5, -2.5f }), Math::Vector2({ 5, 10.0f }), -45.0f, Graphics::Color({ 0.2f, 0.6f, 0.2f, 1.0f }));
        renderer->drawRect(center + Math::Vector2({ 1, 0 }), Math::Vector2({ 5, 15 }), 45.0f, Graphics::Color({ 0.2f, 0.6f, 0.2f, 1.0f }));
    }
}

void Toggle::setSelected(bool selected) { m_isSelected = selected; }
bool Toggle::isSelected() const { return m_isSelected; }
}