#include <UI/Gauge.hpp>

namespace Lib::UI {
Gauge::Gauge()
    : Component()
    , m_minimumValue(0.0f)
    , m_maximumValue(1.0f)
    , m_value(0.0f)
{
}

void Gauge::update() { }
void Gauge::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    auto size = getSize();
    renderer->drawRect(center, size, 0.0f, getBackgroundColor());

    float p = getParcent();
    float fillWidth = size.x() * p;
    center.x() -= size.x() / 2.0f;
    center.x() += fillWidth / 2.0f;
    renderer->drawRect(center, Math::Vector2({ fillWidth, size.y() }), 0.0f, getForegroundColor());
}

void Gauge::setMinimumValue(float minimumValue) { m_minimumValue = minimumValue; }
float Gauge::getMinimumValue() const { return m_minimumValue; }

void Gauge::setMaximumValue(float maximumValue) { m_maximumValue = maximumValue; }
float Gauge::getMaximumValue() const { return m_maximumValue; }

void Gauge::setValue(float value) { m_value = value; }
float Gauge::getValue() const { return m_value; }

float Gauge::getParcent() const
{
    float diff = m_maximumValue - m_minimumValue;
    float relative = m_value - m_minimumValue;
    return relative / diff;
}
}