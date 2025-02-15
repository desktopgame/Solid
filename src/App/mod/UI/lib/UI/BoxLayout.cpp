#include <Math/Mathf.hpp>
#include <UI/BoxLayout.hpp>
#include <UI/Component.hpp>
#include <UI/Container.hpp>
#include <UI/LayoutUtilities.hpp>

namespace Lib::UI {

BoxLayout::BoxLayout(BoxLayout::Orientation orientation)
    : ILayoutManager()
    , m_orientation(orientation)
{
}
void BoxLayout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
    Math::Vector2 minimumSize = parent->getMinimumSize();
    Math::Vector2 maximumSize = parent->getMaximumSize();
    parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
    parent->setPreferredSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
}
void BoxLayout::layoutContainer(const std::shared_ptr<Container>& parent)
{
    int32_t elementCount = parent->getLayoutElementCount();
    Math::Vector2 parentSize = parent->getSize();
    if (elementCount == 0) {
        return;
    }

    auto left = -Math::Vector2({ parent->getSize().x() / 2.0f, 0.0f });
    auto top = Math::Vector2({ 0.0f, parent->getSize().y() / 2.0f });
    float offsetX = k_space;
    float offsetY = -k_space;

    // 残りサイズを取得
    float remainSpace = 0.0f;
    switch (m_orientation) {
    case BoxLayout::Orientation::Horizontal:
        remainSpace = parent->getSize().x() - ((elementCount + 1) * k_space);
        break;
    case BoxLayout::Orientation::Vertical:
        remainSpace = parent->getSize().y() - ((elementCount + 1) * k_space);
        break;
    }

    // フレックスなコンポーネントを数え、フレックスでないサイズ分残りサイズを減らす
    int32_t flexibleComponents = 0;
    for (int32_t i = 0; i < elementCount; i++) {
        auto e = parent->getLayoutElementAt(i);
        auto prefSize = e->component->getPreferredSize();
        auto minSize = e->component->getMinimumSize();
        if (e->component->isFlexible()) {
            switch (m_orientation) {
            case BoxLayout::Orientation::Horizontal:
                prefSize.y() = Math::Mathf::max(minSize.y(), parentSize.y() - (k_space * 2));
                break;
            case BoxLayout::Orientation::Vertical:
                prefSize.x() = Math::Mathf::max(minSize.x(), parentSize.x() - (k_space * 2));
                break;
            }
        }
        e->component->setSize(prefSize);

        if (e->component->isFlexible()) {
            flexibleComponents++;
            continue;
        }

        switch (m_orientation) {
        case BoxLayout::Orientation::Horizontal:
            remainSpace -= prefSize.x();
            break;
        case BoxLayout::Orientation::Vertical:
            remainSpace -= prefSize.y();
            break;
        }
    }

    // フレックスなコンポーネントにサイズを割り当てる
    int32_t div = flexibleComponents;
    for (int32_t i = 0; i < elementCount; i++) {
        auto e = parent->getLayoutElementAt(i);
        auto minimumSize = e->component->getMinimumSize();
        auto maximumSize = e->component->getMaximumSize();

        if (!e->component->isFlexible()) {
            continue;
        }

        Math::Vector2 size;
        switch (m_orientation) {
        case BoxLayout::Orientation::Horizontal: {
            float useWidth = Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), remainSpace / static_cast<float>(div));
            size.x() = Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), useWidth);
            size.y() = Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), parent->getSize().y() - (k_space * 2));
            remainSpace -= useWidth;
            div--;
            break;
        }
        case BoxLayout::Orientation::Vertical: {
            float useHeight = Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), remainSpace / static_cast<float>(div));
            size.x() = Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), parent->getSize().x() - (k_space * 2));
            size.y() = Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), useHeight);
            remainSpace -= useHeight;
            div--;
            break;
        }
        }
        e->component->setSize(size);
    }

    for (int32_t i = 0; i < elementCount; i++) {
        auto e = parent->getLayoutElementAt(i);
        auto size = e->component->getSize();

        switch (m_orientation) {
        case BoxLayout::Orientation::Horizontal:
            e->component->setPosition(left + Math::Vector2({ offsetX + (size.x() / 2.0f), 0.0f }));
            offsetX += size.x();
            offsetX += k_space;
            break;
        case BoxLayout::Orientation::Vertical:
            e->component->setPosition(top + Math::Vector2({ 0.0f, offsetY - (size.y() / 2.0f) }));
            offsetY -= size.y();
            offsetY -= k_space;
            break;
        }
    }
}
Math::Vector2 BoxLayout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    int32_t elementCount = parent->getLayoutElementCount();
    if (elementCount == 0) {
        return Math::Vector2({ 0, 0 });
    }
    Math::Vector2 requiredSize;
    float maxWidth = 0.0f;
    float maxHeight = 0.0f;
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto e = parent->getLayoutElementAt(i);
        auto prefSize = e->component->getPreferredSize();
        if (Math::Mathf::equals(prefSize.x(), 0.0f) || Math::Mathf::equals(prefSize.y(), 0.0f)) {
            auto container = std::dynamic_pointer_cast<Container>(e->component);
            if (container) {
                auto layout = container->getLayout();
                if (layout) {
                    prefSize = layout->computePreferredSize(container);
                }
            }
        }

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
    return requiredSize;
}
Math::Vector2 BoxLayout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    int32_t elementCount = parent->getLayoutElementCount();
    Math::Vector2 parentSize = parent->getSize();
    if (elementCount == 0) {
        return parentSize;
    }

    auto availableSize = container->getPreferredSize();
    if (Math::Mathf::equals(availableSize.x(), 0.0f) || Math::Mathf::equals(availableSize.y(), 0.0f)) {
        auto layout = container->getLayout();
        if (layout) {
            availableSize = layout->computePreferredSize(container);
        }
    }

    switch (m_orientation) {
    case BoxLayout::Orientation::Horizontal:
        availableSize.y() = Math::Mathf::max(availableSize.y(), parentSize.y());
        break;
    case BoxLayout::Orientation::Vertical:
        availableSize.x() = Math::Mathf::max(availableSize.x(), parentSize.x());
        break;
    }
    return availableSize;
}
}