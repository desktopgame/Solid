#pragma once
#include <Graphics/FontMap.hpp>
#include <UI/Box.hpp>
#include <UI/Button.hpp>
#include <UI/Label.hpp>
#include <UI/Panel.hpp>
#include <UI/Slider.hpp>
#include <UI/Toggle.hpp>
#include <string>

namespace Lib::UI {
class Form : public Panel {
public:
    explicit Form(const std::shared_ptr<Graphics::FontMap>& fontMap, int32_t fontSize, Math::Vector2 labelPreferredSize);

    template <typename T, typename... Args>
    std::shared_ptr<T> addAny(const std::u16string& labelText, Args... args)
    {
        auto hbox = Box::createHorizontalBox();

        auto label = std::make_shared<Label>();
        label->setFont(getFont());
        label->setFontSize(getFontSize());
        label->setText(labelText);
        label->setPreferredSize(m_labelPreferredSize);

        auto comp = std::make_shared<T>(args...);
        comp->setFont(getFont());
        comp->setFontSize(getFontSize());
        comp->setFlexible(true);

        hbox->addLayoutElement(std::make_shared<LayoutElement>(label, nullptr));
        hbox->addLayoutElement(std::make_shared<LayoutElement>(comp, nullptr));
        addLayoutElement(std::make_shared<LayoutElement>(hbox, nullptr));
        return comp;
    }

    template <typename T, typename... Args>
    std::shared_ptr<T> addLine(Args... args)
    {
        auto line = std::make_shared<T>(args...);
        addLayoutElement(std::shared_ptr<LayoutElement>(line, nullptr));
        return line;
    }

    std::shared_ptr<Label> addLabel(const std::u16string& label, const std::u16string& text);

    std::shared_ptr<Button> addButton(const std::u16string& label, const std::u16string& text, const std::function<void()>& onClick);

    std::shared_ptr<Toggle> addToggle(const std::u16string& label, const std::function<void(bool)>& onSelected);

    std::shared_ptr<Slider> addSlider(const std::u16string& label, const std::function<void(float)>& onValueChanged);

private:
    Math::Vector2 m_labelPreferredSize;
};
}