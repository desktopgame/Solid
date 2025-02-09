#pragma once
#include <Graphics/Color.hpp>
#include <UI/Component.hpp>

namespace Lib::UI {
class ColorIcon : public Component {
public:
    explicit ColorIcon();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

private:
};
}