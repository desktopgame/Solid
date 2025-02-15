#include <UI/Container.hpp>
#include <UI/LayoutUtilities.hpp>

namespace Lib::UI {
Math::Vector2 LayoutUtilities::measurePreferredSize(const std::shared_ptr<Component>& c)
{
    if (!c) {
        return Math::Vector2({ 0, 0 });
    }
    auto prefSize = c->getPreferredSize();
    if (Math::Mathf::equals(prefSize.x(), 0.0f) || Math::Mathf::equals(prefSize.y(), 0.0f)) {
        auto container = std::dynamic_pointer_cast<Container>(c);
        if (container) {
            auto layout = container->getLayout();
            prefSize = layout->computePreferredSize(container);
        }
    }
    return prefSize;
}
}