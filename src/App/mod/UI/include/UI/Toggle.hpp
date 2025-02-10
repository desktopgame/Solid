#pragma once
#include <UI/Component.hpp>

namespace Lib::UI {
class Toggle : public Component {
public:
    explicit Toggle();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setSelected(bool selected);
    bool isSelected() const;

private:
    bool m_isSelected;
};
}