#include <UI/Button.hpp>

namespace Lib::UI {

Button::Button() { }

void Button::update() { }
void Button::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    renderer->drawRect(center, getSize(), 0.0f, Graphics::Color({ 0, 0, 0, 1 }));
    renderer->drawRect(center, getSize() - Math::Vector2({ 2, 2 }), 0.0f, getBackgroundColor());
    renderer->textFont(getFont());
    renderer->textFontSize(getFontSize());
    renderer->drawText(center, Graphics::Renderer::TextAlignX::Center, Graphics::Renderer::TextAlignY::Center, m_text, getForegroundColor());
}

void Button::setText(const std::u16string& text) { m_text = text; }
std::u16string Button::getText() const { return m_text; }

}