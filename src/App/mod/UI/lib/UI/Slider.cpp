#include <Graphics/Screen.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Mouse.hpp>
#include <Math/Mathf.hpp>
#include <UI/Slider.hpp>

namespace Lib::UI {

Slider::Slider()
    : Component()
    , m_value()
    , m_status(0)
    , m_warp(false)
{
}

void Slider::update()
{
    auto inputSystem = Input::InputSystem::getInstance();
    auto mousePos = inputSystem->getMouse()->getPosition();
    auto mouseStatus = inputSystem->getMouse()->getState(Input::Mouse::Button::Left);

    auto screenPos = (((Math::Vector2)mousePos / (Math::Vector2)Graphics::Screen::getSize()) - Math::Vector2({ 0.5f, 0.5f })) * (Math::Vector2)Graphics::Screen::getSize();
    screenPos.y() *= -1;

    auto center = getGlobalPosition();
    auto size = getSize();
    auto knobCenter = center + Math::Vector2({ (size.x() - 10.0f) * (m_value - 0.5f), 0 });
    auto knobSize = Math::Vector2({ 10, size.y() });
    float knobLeft = knobCenter.x() - (knobSize.x() / 2.0f);
    float knobRight = knobCenter.x() + (knobSize.x() / 2.0f);
    float knobBottom = knobCenter.y() - (knobSize.y() / 2.0f);
    float knobTop = knobCenter.y() + (knobSize.y() / 2.0f);

    bool knobHover = true;
    m_status = k_sliderNone;
    if (screenPos.x() < knobLeft || screenPos.x() > knobRight) {
        knobHover = false;
    }
    if (screenPos.y() < knobBottom || screenPos.y() > knobTop) {
        knobHover = false;
    }

    if (knobHover) {
        m_status = k_sliderHover;

        if (mouseStatus == Input::ButtonState::Trigger) {
            m_warp = true;
        }
        if (mouseStatus == Input::ButtonState::Pressed && m_warp) {
            m_status = k_sliderActive;
            setValue(m_value + ((screenPos.x() - knobCenter.x()) / size.x()));
        }
    } else if (m_warp) {
        if (mouseStatus == Input::ButtonState::Pressed) {
            m_status = k_sliderActive;
            setValue(m_value + ((screenPos.x() - knobCenter.x()) / size.x()));
        }
    }
    if (mouseStatus == Input::ButtonState::Released) {
        m_warp = false;
    }
}
void Slider::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    auto size = getSize();
    renderer->drawRect(center, Math::Vector2({ size.x(), 10 }), 0.0f, Graphics::Color({ 0.0f, 0.0f, 0.0f, 1.0f }));

    switch (m_status) {
    case k_sliderNone:
        renderer->drawRect(center + Math::Vector2({ (size.x() - 10.0f) * (m_value - 0.5f), 0 }), Math::Vector2({ 10, size.y() }), 0.0f, Graphics::Color({ 0.5f, 0.5f, 0.5f, 1.0f }));
        break;
    case k_sliderHover:
        renderer->drawRect(center + Math::Vector2({ (size.x() - 10.0f) * (m_value - 0.5f), 0 }), Math::Vector2({ 10, size.y() }), 0.0f, Graphics::Color({ 0.4f, 0.6f, 0.4f, 1.0f }));
        break;
    case k_sliderActive:
        renderer->drawRect(center + Math::Vector2({ (size.x() - 10.0f) * (m_value - 0.5f), 0 }), Math::Vector2({ 10, size.y() }), 0.0f, Graphics::Color({ 0.2f, 0.6f, 0.2f, 1.0f }));
        break;
    }
}

void Slider::setValue(float value) { m_value = Math::Mathf::clamp(0.0f, 1.0f, value); }
float Slider::getValue() const { return m_value; }
}