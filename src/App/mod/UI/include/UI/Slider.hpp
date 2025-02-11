#pragma once
#include <UI/Component.hpp>
#include <functional>

namespace Lib::UI {
class Slider : public Component {
public:
    explicit Slider();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setValue(float value);
    float getValue() const;

    void setOnValueChanged(const std::function<void(float)>& onValueChanged);
    std::function<void(float)> getOnValueChanged() const;

private:
    static inline constexpr int32_t k_sliderNone = 0;
    static inline constexpr int32_t k_sliderHover = 1;
    static inline constexpr int32_t k_sliderActive = 2;

    float m_value;
    int32_t m_status;
    bool m_warp;
    std::function<void(float)> m_onValueChanged;
};
}