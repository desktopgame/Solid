#include <Graphics/Screen.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Mouse.hpp>
#include <UI/Button.hpp>

namespace Lib::UI {

Button::Button()
    : Component()
    , m_text()
    , m_status(0)
{
}

void Button::update()
{
    auto inputSystem = Input::InputSystem::getInstance();
    auto mousePos = inputSystem->getMouse()->getPosition();
    auto mouseStatus = inputSystem->getMouse()->getState(Input::Mouse::Button::Left);

    auto screenPos = (((Math::Vector2)mousePos / (Math::Vector2)Graphics::Screen::getSize()) - Math::Vector2({ 0.5f, 0.5f })) * (Math::Vector2)Graphics::Screen::getSize();
    screenPos.y() *= -1;

    if (isContains(screenPos)) {
        switch (mouseStatus) {
        case Input::ButtonState::None:
            m_status = k_buttonNone;
            break;
        case Input::ButtonState::Pressed:
            m_status = k_buttonActive;
            break;
        case Input::ButtonState::Released:
            m_status = k_buttonHover;
            break;
        case Input::ButtonState::Trigger:
            m_status = k_buttonActive;

            if (m_onClick) {
                m_onClick();
            }
            break;
        }
    } else {
        m_status = k_buttonNone;
    }
}
void Button::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    renderer->drawRect(center, getSize(), 0.0f, Graphics::Color({ 0, 0, 0, 1 }));

    switch (m_status) {
    case k_buttonNone:
        renderer->drawRect(center, getSize() - Math::Vector2({ 2, 2 }), 0.0f, getBackgroundColor());
        break;
    case k_buttonHover:
        renderer->drawRect(center, getSize() - Math::Vector2({ 2, 2 }), 0.0f, Graphics::Color({ 0.2f, 0.6f, 0.2f, 1.0f }));
        break;
    case k_buttonActive:
        renderer->drawRect(center, getSize() - Math::Vector2({ 2, 2 }), 0.0f, Graphics::Color({ 0.4f, 0.6f, 0.4f, 1.0f }));
        break;
    }
    renderer->textFont(getFont());
    renderer->textFontSize(getFontSize());
    renderer->drawText(center, Graphics::Renderer::TextAlignX::Center, Graphics::Renderer::TextAlignY::Center, m_text, getForegroundColor());
}

void Button::setText(const std::u16string& text) { m_text = text; }
std::u16string Button::getText() const { return m_text; }

void Button::setOnClick(const std::function<void()>& onClick) { m_onClick = onClick; }
std::function<void()> Button::getOnClick() const { return m_onClick; }
}