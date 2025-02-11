#include <UI/BoxLayout.hpp>
#include <UI/Form.hpp>

namespace Lib::UI {
// public
Form::Form(const std::shared_ptr<Graphics::FontMap>& fontMap, int32_t fontSize, Math::Vector2 labelPreferredSize)
    : Panel()
    , m_labelPreferredSize(labelPreferredSize)
{
    setFont(fontMap);
    setFontSize(fontSize);
    setLayout(std::make_shared<BoxLayout>(BoxLayout::Orientation::Vertical));
}

std::shared_ptr<Label> Form::addLabel(const std::u16string& label, const std::u16string& text)
{
    auto valueLabel = addAny<Label>(label);
    valueLabel->setText(text);
    return valueLabel;
}

std::shared_ptr<Button> Form::addButton(const std::u16string& label, const std::u16string& text, const std::function<void()>& onClick)
{
    auto button = addAny<Button>(label);
    button->setText(text);
    button->setOnClick(onClick);
    return button;
}

std::shared_ptr<Toggle> Form::addToggle(const std::u16string& label, const std::function<void(bool)>& onSelected)
{
    auto toggle = addAny<Toggle>(label);
    toggle->setOnSelected(onSelected);
    return toggle;
}

std::shared_ptr<Slider> Form::addSlider(const std::u16string& label, const std::function<void(float)>& onValueChanged)
{
    auto slider = addAny<Slider>(label);
    slider->setOnValueChanged(onValueChanged);
    return slider;
}

}