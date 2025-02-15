#pragma once
#include <UI/Component.hpp>

namespace Lib::UI {
class LayoutUtilities {
public:
    static Math::Vector2 measurePreferredSize(const std::shared_ptr<Component>& c);

private:
    LayoutUtilities() = delete;
    ~LayoutUtilities() = delete;
};
}