#include <UI/Container.hpp>
#include <UI/LayoutUtilities.hpp>

namespace Lib::UI {
Math::Vector2 LayoutUtilities::measurePreferredSize(const std::shared_ptr<Component>& c)
{
    if (!c) {
        return Math::Vector2({ 0, 0 });
    }
    auto prefSize = c->getPreferredSize();
    auto container = std::dynamic_pointer_cast<Container>(c);
    if (container) {
        auto layout = container->getLayout();
        if (layout) {
            prefSize = layout->computePreferredSize(container);
        }
    }
    return prefSize;
}
}