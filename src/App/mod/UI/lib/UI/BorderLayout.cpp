#include <UI/BorderLayout.hpp>
#include <UI/Container.hpp>

namespace Lib::UI {
// public
void BorderLayout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
    Math::Vector2 minimumSize = parent->getMinimumSize();
    Math::Vector2 maximumSize = parent->getMaximumSize();
    parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
    parent->setPreferredSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
}
void BorderLayout::layoutContainer(const std::shared_ptr<Container>& parent) { }
Math::Vector2 BorderLayout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    return Math::Vector2({ 0, 0 });
}
Math::Vector2 BorderLayout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    return Math::Vector2({ 0, 0 });
}
// private
bool BorderLayout::findLocation(const std::shared_ptr<Container>& parent, const std::string& location)
{
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        if (std::static_pointer_cast<BorderLayout::Hint>(parent->getLayoutElementAt(i)->hint)->location == location) {
            return true;
        }
    }
    return false;
}
}