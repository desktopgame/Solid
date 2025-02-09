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
        return;
    }
    Math::Vector2 requiredSize;
    float maxWidth = 0.0f;
    float maxHeight = 0.0f;
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto e = parent->getLayoutElementAt(i);
        auto prefSize = e->component->getPreferredSizeSize();

        switch (m_orientation) {
        case BoxLayout::Orientation::Horizontal:
            requiredSize.x() += prefSize.x();
            if (maxHeight < prefSize.y()) {
                maxHeight = prefSize.y();
            }
            break;
        case BoxLayout::Orientation::Vertical:
            requiredSize.y() += prefSize.y();
            if (maxWidth < prefSize.x()) {
                maxWidth = prefSize.x();
            }
            break;
        }
    }

    switch (m_orientation) {
    case BoxLayout::Orientation::Horizontal:
        requiredSize.x() += (elementCount + 1) * k_space;
        requiredSize.y() = maxHeight + (k_space * 2);
        break;
    case BoxLayout::Orientation::Vertical:
        requiredSize.y() += (elementCount + 1) * k_space;
        requiredSize.x() = maxWidth + (k_space * 2);
        break;
    }

    requiredSize.x() = Math::Mathf::min(requiredSize.x(), availableSize.x());
    requiredSize.y() = Math::Mathf::min(requiredSize.y(), availableSize.y());

    Math::Vector2 minimumSize = parent->getMinimumSize();
    Math::Vector2 maximumSize = parent->getMaximumSize();
    parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), requiredSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), requiredSize.y()) }));
}
void BoxLayout::layoutContainer(const std::shared_ptr<Container>& parent)
{
    auto left = parent->getPosition() - Math::Vector2({ parent->getSize().x() / 2.0f, 0.0f });
    auto top = parent->getPosition() + Math::Vector2({ 0.0f, parent->getSize().y() / 2.0f });
    float offsetX = k_space;
    float offsetY = -k_space;
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto e = parent->getLayoutElementAt(i);
        auto prefSize = e->component->getPreferredSizeSize();
        e->component->setSize(prefSize);

        switch (m_orientation) {
        case BoxLayout::Orientation::Horizontal:
            e->component->setPosition(left + Math::Vector2({ offsetX, 0.0f }));
            offsetX += prefSize.x();
            offsetX += k_space;
            break;
        case BoxLayout::Orientation::Vertical:
            e->component->setPosition(top - Math::Vector2({ 0.0f, offsetY }));
            offsetY -= prefSize.y();
            offsetY -= k_space;
            break;
        }
    }
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