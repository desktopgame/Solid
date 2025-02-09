#include <UI/LayoutElement.hpp>

namespace Lib::UI {

LayoutElement::LayoutElement(const std::shared_ptr<Component>& component, const std::shared_ptr<LayoutHint>& hint)
    : component(component)
    , hint(hint)
{
}
}