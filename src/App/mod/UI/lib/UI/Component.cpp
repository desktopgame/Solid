#include <UI/Component.hpp>
#include <UI/Container.hpp>

namespace Lib::UI {
// public
Component::Component()
    : m_parent()
    , m_size()
    , m_position()
    , m_minimumSize({ 0, 0 })
    , m_maximumSize({ 9999, 9999 })
    , m_preferredSize({ 0, 0 })
    , m_isFlexible(false)
    , m_foregroundColor({ 1.0f, 1.0f, 1.0f, 1.0f })
    , m_backgroundColor({ 0.2f, 0.2f, 0.2f, 1.0f })
    , m_fontMap()
    , m_fontSize(16)
    , m_name()
{
}
Component::~Component() { }

void Component::setParent(const std::shared_ptr<Container>& parent) { m_parent = parent; }
std::shared_ptr<Container> Component::getParent() const { return m_parent.lock(); }

void Component::setSize(const Math::Vector2& size) { m_size = size; }
Math::Vector2 Component::getSize() const { return m_size; }

void Component::setPosition(const Math::Vector2& position) { m_position = position; }
Math::Vector2 Component::getPosition() const { return m_position; }

Math::Vector2 Component::getGlobalPosition() const
{
    auto spParent = m_parent.lock();
    if (spParent) {
        return spParent->getGlobalPosition() + m_position;
    }
    return m_position;
}

bool Component::isContains(const Math::Vector2& position)
{
    Math::Vector2 center = getGlobalPosition();
    float width = m_size.x();
    float height = m_size.y();
    float left = center.x() - (width / 2.0f);
    float top = center.y() + (height / 2.0f);
    float right = center.x() + (width / 2.0f);
    float bottom = center.y() - (height / 2.0f);

    if (position.x() < left || position.x() > right) {
        return false;
    }

    if (position.y() < bottom || position.y() > top) {
        return false;
    }
    return true;
}

void Component::setMinimumSize(const Math::Vector2& minimumSize) { m_minimumSize = minimumSize; }
Math::Vector2 Component::getMinimumSize() const { return m_minimumSize; }

void Component::setMaximumSize(const Math::Vector2& maximumSize) { m_maximumSize = maximumSize; }
Math::Vector2 Component::getMaximumSize() const { return m_maximumSize; }

void Component::setPreferredSize(const Math::Vector2& preferredSize) { m_preferredSize = preferredSize; }
Math::Vector2 Component::getPreferredSize() const { return m_preferredSize; }

void Component::setFlexible(bool isFlexible) { m_isFlexible = isFlexible; }
bool Component::isFlexible() const { return m_isFlexible; }

void Component::setForegroundColor(const Graphics::Color& foregroundColor) { m_foregroundColor = foregroundColor; }
Graphics::Color Component::getForegroundColor() const { return m_foregroundColor; }

void Component::setBackgroundColor(const Graphics::Color& backgroundColor) { m_backgroundColor = backgroundColor; }
Graphics::Color Component::getBackgroundColor() const { return m_backgroundColor; }

void Component::setFont(const std::shared_ptr<Graphics::FontMap>& fontMap) { m_fontMap = fontMap; }
std::shared_ptr<Graphics::FontMap> Component::getFont() const { return m_fontMap; }

void Component::setFontSize(int32_t fontSize) { m_fontSize = fontSize; }
int32_t Component::getFontSize() const { return m_fontSize; }

void Component::setName(const std::string& name) { m_name = name; }
std::string Component::getName() const { return m_name; }
}