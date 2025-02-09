#include <UI/Label.hpp>

namespace Lib::UI {
Label::Label()
    : Component()
    , m_text()
{
}

void Label::update() { }
void Label::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->textFont(getFont());
    renderer->textFontSize(getFontSize());
    renderer->drawText(getGlobalPosition(), Graphics::Renderer::TextAlignX::Center, Graphics::Renderer::TextAlignY::Center, m_text, getForegroundColor());
}

void Label::setText(const std::u16string& text) { m_text = text; }
std::u16string Label::getText() const { return m_text; }
};