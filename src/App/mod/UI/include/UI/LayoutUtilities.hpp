#pragma once
#include <UI/Component.hpp>
#include <UI/Container.hpp>

namespace Lib::UI {
class LayoutUtilities {
public:
    static Math::Vector2 measurePreferredSize(const std::shared_ptr<Component>& c);

    static Math::Vector2 measurePreferredSize(const std::shared_ptr<Container>& c);

private:
    LayoutUtilities() = delete;
    ~LayoutUtilities() = delete;
};
}