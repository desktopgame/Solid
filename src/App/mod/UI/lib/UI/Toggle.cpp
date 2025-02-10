#include <Graphics/Screen.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Mouse.hpp>
#include <UI/Toggle.hpp>

namespace Lib::UI {

Toggle::Toggle()
    : Component()
    , m_isSelected(false)
{
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
    auto size = getSize();
    renderer->drawRect(center, size, 0.0f, Graphics::Color({ 0, 0, 0, 1 }));
    renderer->drawRect(center, size - Math::Vector2({ 2, 2 }), 0.0f, getBackgroundColor());
    renderer->drawRect(center, size - Math::Vector2({ 10, 10 }), 0.0f, Graphics::Color({ 0.5f, 0.5f, 0.5f, 1.0f }));

    Math::Vector2 cellSize = Math::Vector2({ (size.x() - 10.0f) / 2.0f, size.y() - 10.0f });
    if (isSelected()) {
        Math::Vector2 cellOffset = Math::Vector2({ size.x() / 2.0f, 0 }) - Math::Vector2({ (cellSize.x() / 2.0f) + 5.0f, 0 });
        renderer->drawRect(center + cellOffset, cellSize, 0.0f, Graphics::Color({ 0.2f, 0.6f, 0.2f, 1.0f }));
    } else {
        Math::Vector2 cellOffset = -Math::Vector2({ size.x() / 2.0f, 0 }) + Math::Vector2({ (cellSize.x() / 2.0f) + 5.0f, 0 });
        renderer->drawRect(center + cellOffset, cellSize, 0.0f, Graphics::Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
    }
}

void Toggle::setSelected(bool selected) { m_isSelected = selected; }
bool Toggle::isSelected() const { return m_isSelected; }
}