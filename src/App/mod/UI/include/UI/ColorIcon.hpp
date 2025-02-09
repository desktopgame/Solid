#pragma once
#include <Graphics/Color.hpp>
#include <UI/Component.hpp>

namespace Lib::UI {
class ColorIcon : public Component {
public:
    explicit ColorIcon();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setColor(const Graphics::Color& color);
    Graphics::Color getColor() const;

private:
    Graphics::Color m_color;
};
}