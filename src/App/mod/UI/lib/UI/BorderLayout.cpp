#include <UI/BorderLayout.hpp>
#include <UI/Container.hpp>

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
        auto newSize = top->getPreferredSizeSize();

        if (top->isFlexible()) {
            newSize.x() = parentSize.x();
        }
        top->setSize(newSize);
        top->setPosition(Math::Vector2({ 0, (parentSize.y() / 2.0f) - (newSize.y() / 2.0f) }));
    }
    // BOTTOM
    auto bottom = findByLocation(parent, "BOTTOM");
    if (bottom) {
        auto newSize = bottom->getPreferredSizeSize();

        if (bottom->isFlexible()) {
            newSize.x() = parentSize.x();
        }
        bottom->setSize(newSize);
        bottom->setPosition(Math::Vector2({ 0, -(parentSize.y() / 2.0f) + (newSize.y() / 2.0f) }));
    }
    // LEFT
    auto left = findByLocation(parent, "LEFT");
    if (left) {
        if (left->isFlexible()) {
            auto newSize = left->getPreferredSizeSize();
            Math::Vector2 newPos;

            if (top && bottom) {
                newSize.y() = parentSize.y() - top->getPreferredSizeSize().y() - bottom->getPreferredSizeSize().y();
                newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSizeSize().y() / 2.0f) + (newSize.y() / 2.0f);
            } else if (top) {
                newSize.y() = parentSize.y() - top->getPreferredSizeSize().y();
                newPos.y() = top->getPosition().y() - (top->getPreferredSizeSize().y() / 2.0f) - newSize.y() / 2.0f;
            } else if (bottom) {
                newSize.y() = parentSize.y() - bottom->getPreferredSizeSize().y();
                newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSizeSize().y() / 2.0f) + newSize.y() / 2.0f;
            } else {
                newSize.y() = parentSize.y();
            }
            newPos.x() = (-parentSize.x() / 2.0f) + newSize.x() / 2.0f;
            left->setPosition(newPos);
            left->setSize(newSize);
        } else {
            left->setPosition(Math::Vector2({ 0, 0 }));
            left->setSize(left->getPreferredSizeSize());
        }
    }
    // RIGHT
    auto right = findByLocation(parent, "RIGHT");
    if (right) {
        if (right->isFlexible()) {
            auto newSize = right->getPreferredSizeSize();
            Math::Vector2 newPos;

            if (top && bottom) {
                newSize.y() = parentSize.y() - top->getPreferredSizeSize().y() - bottom->getPreferredSizeSize().y();
                newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSizeSize().y() / 2.0f) + (newSize.y() / 2.0f);
            } else if (top) {
                newSize.y() = parentSize.y() - top->getPreferredSizeSize().y();
                newPos.y() = top->getPosition().y() - (top->getPreferredSizeSize().y() / 2.0f) - newSize.y() / 2.0f;
            } else if (bottom) {
                newSize.y() = parentSize.y() - bottom->getPreferredSizeSize().y();
                newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSizeSize().y() / 2.0f) + newSize.y() / 2.0f;
            } else {
                newSize.y() = parentSize.y();
            }
            newPos.x() = (parentSize.x() / 2.0f) - newSize.x() / 2.0f;
            right->setPosition(newPos);
            right->setSize(newSize);
        } else {
            right->setPosition(Math::Vector2({ 0, 0 }));
            right->setSize(right->getPreferredSizeSize());
        }
    }
    // CENTER
    auto center = findByLocation(parent, "CENTER");
    if (center) {

        if (center->isFlexible()) {
            auto newSize = center->getPreferredSizeSize();
            Math::Vector2 newPos;

            if (top && bottom) {
                newSize.y() = parentSize.y() - top->getPreferredSizeSize().y() - bottom->getPreferredSizeSize().y();
                newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSizeSize().y() / 2.0f) + (newSize.y() / 2.0f);
            } else if (top) {
                newSize.y() = parentSize.y() - top->getPreferredSizeSize().y();
                newPos.y() = top->getPosition().y() - (top->getPreferredSizeSize().y() / 2.0f) - newSize.y() / 2.0f;
            } else if (bottom) {
                newSize.y() = parentSize.y() - bottom->getPreferredSizeSize().y();
                newPos.y() = bottom->getPosition().y() + (bottom->getPreferredSizeSize().y() / 2.0f) + newSize.y() / 2.0f;
            } else {
                newSize.y() = parentSize.y();
            }
            if (left && right) {
                newSize.x() = parentSize.x() - left->getPreferredSizeSize().x() - right->getPreferredSizeSize().x();
                newPos.x() = left->getPosition().x() + (left->getPreferredSizeSize().x() / 2.0f) + (newSize.x() / 2.0f);
            } else if (left) {
                newSize.x() = parentSize.x() - left->getPreferredSizeSize().x();
                newPos.x() = left->getPosition().x() + (left->getPreferredSizeSize().x() / 2.0f) + newSize.x() / 2.0f;
            } else if (right) {
                newSize.x() = parentSize.x() - right->getPreferredSizeSize().x();
                newPos.x() = right->getPosition().x() - (right->getPreferredSizeSize().x() / 2.0f) - newSize.x() / 2.0f;
            } else {
                newSize.x() = parentSize.x();
            }
            center->setPosition(newPos);
            center->setSize(newSize);
        } else {
            center->setPosition(Math::Vector2({ 0, 0 }));
            center->setSize(center->getPreferredSizeSize());
        }
    }
}
Math::Vector2 BorderLayout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    Math::Vector2 preferredSize;
    auto top = findByLocation(parent, "TOP");
    auto topPS = top ? top->getPreferredSizeSize() : Math::Vector2();
    auto bottom = findByLocation(parent, "BOTTOM");
    auto bottomPS = bottom ? bottom->getPreferredSizeSize() : Math::Vector2();
    auto left = findByLocation(parent, "LEFT");
    auto leftPS = left ? left->getPreferredSizeSize() : Math::Vector2();
    auto right = findByLocation(parent, "RIGHT");
    auto rightPS = right ? right->getPreferredSizeSize() : Math::Vector2();
    auto center = findByLocation(parent, "CENTER");
    auto centerPS = center ? center->getPreferredSizeSize() : Math::Vector2();

    preferredSize.x() = leftPS.y() + Math::Mathf::max(leftPS.y(), Math::Mathf::max(centerPS.y(), rightPS.y())) + rightPS.y();
    preferredSize.y() = topPS.y() + Math::Mathf::max(topPS.y(), Math::Mathf::max(centerPS.y(), bottomPS.y())) + bottomPS.y();
    return preferredSize;
}
Math::Vector2 BorderLayout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    auto borderLayoutHint = std::static_pointer_cast<BorderLayout::Hint>(hint);
    Math::Vector2 parentSize = parent->getSize();
    Math::Vector2 availableSize;

    auto preferredSizeSize = container->getPreferredSizeSize();
    if (Math::Mathf::equals(availableSize.x(), 0.0f) || Math::Mathf::equals(availableSize.y(), 0.0f)) {
        auto layout = container->getLayout();
        if (layout) {
            preferredSizeSize = layout->computePreferredSize(container);
        }
    }

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
            remainHeight -= top->getPreferredSizeSize().y();
        }
        auto bottom = findByLocation(parent, "BOTTOM");
        if (bottom) {
            remainHeight -= bottom->getPreferredSizeSize().y();
        }
        availableSize.y() = remainHeight;
    } else if (borderLayoutHint->location == "RIGHT") {
        availableSize.x() = preferredSizeSize.x();

        float remainHeight = parentSize.y();
        auto top = findByLocation(parent, "TOP");
        if (top) {
            remainHeight -= top->getPreferredSizeSize().y();
        }
        auto bottom = findByLocation(parent, "BOTTOM");
        if (bottom) {
            remainHeight -= bottom->getPreferredSizeSize().y();
        }
        availableSize.y() = remainHeight;
    } else if (borderLayoutHint->location == "CENTER") {
        float remainWidth = parentSize.x();
        float remainHeight = parentSize.y();

        auto top = findByLocation(parent, "TOP");
        if (top) {
            remainHeight -= top->getPreferredSizeSize().y();
        }
        auto bottom = findByLocation(parent, "BOTTOM");
        if (bottom) {
            remainHeight -= bottom->getPreferredSizeSize().y();
        }

        auto left = findByLocation(parent, "LEFT");
        if (left) {
            remainWidth -= left->getPreferredSizeSize().x();
        }
        auto right = findByLocation(parent, "RIGHT");
        if (right) {
            remainWidth -= right->getPreferredSizeSize().x();
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