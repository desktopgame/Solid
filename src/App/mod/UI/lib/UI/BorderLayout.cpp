#include <UI/BorderLayout.hpp>
#include <UI/Container.hpp>
#include <UI/LayoutUtilities.hpp>

namespace Lib::UI {
// Hint
BorderLayout::Hint::Hint(const std::string& location)
    : location(location)
{
}
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
void BorderLayout::layoutContainer(const std::shared_ptr<Container>& parent)
{
    int32_t elementCount = parent->getLayoutElementCount();
    Math::Vector2 parentSize = parent->getSize();
    if (elementCount == 0) {
        return;
    }

    // TOP
    auto top = findByLocation(parent, "TOP");
    if (top) {
        auto newSize = top->getPreferredSize();

        if (top->isFlexible()) {
            newSize.x() = parentSize.x();
        }
        top->setSize(newSize);
        top->setPosition(Math::Vector2({ 0, (parentSize.y() / 2.0f) - (newSize.y() / 2.0f) }));
    }
    // BOTTOM
    auto bottom = findByLocation(parent, "BOTTOM");
    if (bottom) {
        auto newSize = bottom->getPreferredSize();

        if (bottom->isFlexible()) {
            newSize.x() = parentSize.x();
        }
        bottom->setSize(newSize);
        bottom->setPosition(Math::Vector2({ 0, -(parentSize.y() / 2.0f) + (newSize.y() / 2.0f) }));
    }
    // LEFT
    auto left = findByLocation(parent, "LEFT");
    if (left) {
        auto newSize = left->getPreferredSize();
        Math::Vector2 newPos;

        if (top && bottom) {
            if (left->isFlexible()) {
                newSize.y() = parentSize.y() - top->getPreferredSize().y() - bottom->getPreferredSize().y();
            }
            newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSize().y() / 2.0f) + (newSize.y() / 2.0f);
        } else if (top) {
            if (left->isFlexible()) {
                newSize.y() = parentSize.y() - top->getPreferredSize().y();
            }
            newPos.y() = top->getPosition().y() - (top->getPreferredSize().y() / 2.0f) - newSize.y() / 2.0f;
        } else if (bottom) {
            if (left->isFlexible()) {
                newSize.y() = parentSize.y() - bottom->getPreferredSize().y();
            }
            newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSize().y() / 2.0f) + newSize.y() / 2.0f;
        } else {
            if (left->isFlexible()) {
                newSize.y() = parentSize.y();
            }
        }
        newPos.x() = (-parentSize.x() / 2.0f) + newSize.x() / 2.0f;
        left->setPosition(newPos);
        left->setSize(newSize);
    }
    // RIGHT
    auto right = findByLocation(parent, "RIGHT");
    if (right) {
        auto newSize = right->getPreferredSize();
        Math::Vector2 newPos;

        if (top && bottom) {
            if (right->isFlexible()) {
                newSize.y() = parentSize.y() - top->getPreferredSize().y() - bottom->getPreferredSize().y();
            }
            newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSize().y() / 2.0f) + (newSize.y() / 2.0f);
        } else if (top) {
            if (right->isFlexible()) {
                newSize.y() = parentSize.y() - top->getPreferredSize().y();
            }
            newPos.y() = top->getPosition().y() - (top->getPreferredSize().y() / 2.0f) - newSize.y() / 2.0f;
        } else if (bottom) {
            if (right->isFlexible()) {
                newSize.y() = parentSize.y() - bottom->getPreferredSize().y();
            }
            newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSize().y() / 2.0f) + newSize.y() / 2.0f;
        } else {
            if (right->isFlexible()) {
                newSize.y() = parentSize.y();
            }
        }
        newPos.x() = (parentSize.x() / 2.0f) - newSize.x() / 2.0f;
        right->setPosition(newPos);
        right->setSize(newSize);
    }
    // CENTER
    auto center = findByLocation(parent, "CENTER");
    if (center) {
        auto newSize = center->getPreferredSize();
        Math::Vector2 newPos;

        if (top && bottom) {
            if (center->isFlexible()) {
                newSize.y() = parentSize.y() - top->getPreferredSize().y() - bottom->getPreferredSize().y();
            }
            newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSize().y() / 2.0f) + (newSize.y() / 2.0f);
        } else if (top) {
            if (center->isFlexible()) {
                newSize.y() = parentSize.y() - top->getPreferredSize().y();
            }
            newPos.y() = top->getPosition().y() - (top->getPreferredSize().y() / 2.0f) - newSize.y() / 2.0f;
        } else if (bottom) {
            if (center->isFlexible()) {
                newSize.y() = parentSize.y() - bottom->getPreferredSize().y();
            }
            newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSize().y() / 2.0f) + newSize.y() / 2.0f;
        } else {
            if (center->isFlexible()) {
                newSize.y() = parentSize.y();
            }
        }
        if (left && right) {
            if (center->isFlexible()) {
                newSize.x() = parentSize.x() - left->getPreferredSize().x() - right->getPreferredSize().x();
            }
            newPos.x() = left->getPosition().x() + (left->getPreferredSize().x() / 2.0f) + (newSize.x() / 2.0f);
        } else if (left) {
            if (center->isFlexible()) {
                newSize.x() = parentSize.x() - left->getPreferredSize().x();
            }
            newPos.x() = left->getPosition().x() + (left->getPreferredSize().x() / 2.0f) + newSize.x() / 2.0f;
        } else if (right) {
            if (center->isFlexible()) {
                newSize.x() = parentSize.x() - right->getPreferredSize().x();
            }
            newPos.x() = right->getPosition().x() - (right->getPreferredSize().x() / 2.0f) - newSize.x() / 2.0f;
        } else {
            if (center->isFlexible()) {
                newSize.x() = parentSize.x();
            }
        }
        center->setPosition(newPos);
        center->setSize(newSize);
    }
}
Math::Vector2 BorderLayout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    Math::Vector2 preferredSize;
    auto top = findByLocation(parent, "TOP");
    auto topPS = LayoutUtilities::measurePreferredSize(top);
    auto bottom = findByLocation(parent, "BOTTOM");
    auto bottomPS = LayoutUtilities::measurePreferredSize(bottom);
    auto left = findByLocation(parent, "LEFT");
    auto leftPS = LayoutUtilities::measurePreferredSize(left);
    auto right = findByLocation(parent, "RIGHT");
    auto rightPS = LayoutUtilities::measurePreferredSize(right);
    auto center = findByLocation(parent, "CENTER");
    auto centerPS = LayoutUtilities::measurePreferredSize(center);

    preferredSize.x() = leftPS.x() + Math::Mathf::max(leftPS.x(), Math::Mathf::max(centerPS.x(), rightPS.x())) + rightPS.x();
    preferredSize.y() = topPS.y() + Math::Mathf::max(topPS.y(), Math::Mathf::max(centerPS.y(), bottomPS.y())) + bottomPS.y();
    return preferredSize;
}
Math::Vector2 BorderLayout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    auto borderLayoutHint = std::static_pointer_cast<BorderLayout::Hint>(hint);
    Math::Vector2 parentSize = parent->getSize();
    Math::Vector2 availableSize;

    auto preferredSizeSize = LayoutUtilities::measurePreferredSize(container);

    if (borderLayoutHint->location == "TOP") {
        availableSize.x() = parentSize.x();
        availableSize.y() = preferredSizeSize.y();
    } else if (borderLayoutHint->location == "BOTTOM") {
        availableSize.x() = parentSize.x();
        availableSize.y() = preferredSizeSize.y();
    } else if (borderLayoutHint->location == "LEFT") {
        availableSize.x() = preferredSizeSize.x();

        float remainHeight = parentSize.y();
        auto top = findByLocation(parent, "TOP");
        if (top) {
            remainHeight -= LayoutUtilities::measurePreferredSize(top).y();
        }
        auto bottom = findByLocation(parent, "BOTTOM");
        if (bottom) {
            remainHeight -= LayoutUtilities::measurePreferredSize(bottom).y();
        }
        availableSize.y() = remainHeight;
    } else if (borderLayoutHint->location == "RIGHT") {
        availableSize.x() = preferredSizeSize.x();

        float remainHeight = parentSize.y();
        auto top = findByLocation(parent, "TOP");
        if (top) {
            remainHeight -= LayoutUtilities::measurePreferredSize(top).y();
        }
        auto bottom = findByLocation(parent, "BOTTOM");
        if (bottom) {
            remainHeight -= LayoutUtilities::measurePreferredSize(bottom).y();
        }
        availableSize.y() = remainHeight;
    } else if (borderLayoutHint->location == "CENTER") {
        float remainWidth = parentSize.x();
        float remainHeight = parentSize.y();

        auto top = findByLocation(parent, "TOP");
        if (top) {
            remainHeight -= LayoutUtilities::measurePreferredSize(top).y();
        }
        auto bottom = findByLocation(parent, "BOTTOM");
        if (bottom) {
            remainHeight -= LayoutUtilities::measurePreferredSize(bottom).y();
        }

        auto left = findByLocation(parent, "LEFT");
        if (left) {
            remainWidth -= LayoutUtilities::measurePreferredSize(left).x();
        }
        auto right = findByLocation(parent, "RIGHT");
        if (right) {
            remainWidth -= LayoutUtilities::measurePreferredSize(right).x();
        }

        availableSize.x() = remainWidth;
        availableSize.y() = remainHeight;
    }
    return availableSize;
}
// private
std::shared_ptr<Component> BorderLayout::findByLocation(const std::shared_ptr<Container>& parent, const std::string& location)
{
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto element = parent->getLayoutElementAt(i);
        if (std::static_pointer_cast<BorderLayout::Hint>(element->hint)->location == location) {
            return element->component;
        }
    }
    return nullptr;
}
}