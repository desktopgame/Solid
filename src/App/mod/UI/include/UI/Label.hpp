#pragma once
#include <UI/Component.hpp>
#include <string>

namespace Lib::UI {
class Label : public Component {
public:
    explicit Label();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setText(const std::u16string& text);
    std::u16string getText() const;

private:
    std::u16string m_text;
};
}