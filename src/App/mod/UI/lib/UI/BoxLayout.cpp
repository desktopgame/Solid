#include <Math/Mathf.hpp>
#include <UI/BoxLayout.hpp>
#include <UI/Component.hpp>
#include <UI/Container.hpp>

namespace Lib::UI {

BoxLayout::BoxLayout(BoxLayout::Orientation orientation)
    : ILayoutManager()
    , m_orientation(orientation)
{
}
void BoxLayout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
    int32_t elementCount = parent->getLayoutElementCount();
    if (elementCount == 0) {
        parent->setSize(Math::Vector2({ 0, 0 }));
    } else {
        Math::Vector2 minimumSize = parent->getMinimumSize();
        Math::Vector2 maximumSize = parent->getMaximumSize();
        parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
            Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
    }
}
void BoxLayout::layoutContainer(const std::shared_ptr<Container>& parent)
{
}

Math::Vector2 BoxLayout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<LayoutElement>& layoutElement)
{
    int32_t elementCount = parent->getLayoutElementCount();
    Math::Vector2 parentSize = parent->getSize();
    if (elementCount == 0) {
        return parentSize;
    }

    Math::Vector2 availableSize;
    switch (m_orientation) {
    case BoxLayout::Orientation::Horizontal:
        availableSize.x() = (parentSize.x() - ((elementCount + 1) * k_space)) / static_cast<float>(elementCount);
        availableSize.y() = parentSize.y() - (k_space * 2);
        break;
    case BoxLayout::Orientation::Vertical:
        availableSize.x() = parentSize.x() - (k_space * 2);
        availableSize.y() = (parentSize.y() - ((elementCount + 1) * k_space)) / static_cast<float>(elementCount);
        break;
    }
    availableSize.x() = Math::Mathf::max(availableSize.x(), 0.0f);
    availableSize.y() = Math::Mathf::max(availableSize.y(), 0.0f);
    return availableSize;
}
}