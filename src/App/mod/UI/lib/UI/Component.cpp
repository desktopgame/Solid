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

void Component::setMinimumSize(const Math::Vector2& minimumSize) { m_minimumSize = minimumSize; }
Math::Vector2 Component::getMinimumSize() const { return m_minimumSize; }

void Component::setMaximumSize(const Math::Vector2& maximumSize) { m_maximumSize = maximumSize; }
Math::Vector2 Component::getMaximumSize() const { return m_maximumSize; }

void Component::setPreferredSize(const Math::Vector2& preferredSize) { m_preferredSize = preferredSize; }
Math::Vector2 Component::getPreferredSizeSize() const { return m_preferredSize; }
}