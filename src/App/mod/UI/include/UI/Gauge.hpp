#pragma once
#include <UI/Component.hpp>

namespace Lib::UI {
class Gauge : public Component {
public:
    explicit Gauge();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setMinimumValue(float minimumValue);
    float getMinimumValue() const;

    void setMaximumValue(float maximumValue);
    float getMaximumValue() const;

    void setValue(float value);
    float getValue() const;

    float getParcent() const;

private:
    float m_minimumValue;
    float m_maximumValue;
    float m_value;
};
}