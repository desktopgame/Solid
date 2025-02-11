#pragma once
#include <UI/Component.hpp>
#include <functional>

namespace Lib::UI {
class Toggle : public Component {
public:
    explicit Toggle();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setSelected(bool selected);
    bool isSelected() const;

    void setOnSelected(const std::function<void(bool)> onSelected);
    std::function<void(bool)> getOnSelected() const;

private:
    static inline constexpr float k_width = 25.0f;
    static inline constexpr float k_height = 25.0f;
    bool m_isSelected;
    std::function<void(bool)> m_onSelected;
};
}