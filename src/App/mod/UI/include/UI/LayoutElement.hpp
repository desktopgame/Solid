#pragma once
#include <memory>

namespace Lib::UI {
class Component;
class LayoutHint;
class LayoutElement {
public:
    explicit LayoutElement(const std::shared_ptr<Component>& component, const std::shared_ptr<LayoutHint>& hint);
    const std::shared_ptr<Component> component;
    const std::shared_ptr<LayoutHint> hint;

private:
};
}