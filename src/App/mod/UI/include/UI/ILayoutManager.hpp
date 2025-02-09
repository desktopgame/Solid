#pragma once
#include <Math/Vector.hpp>
#include <UI/LayoutElement.hpp>
#include <memory>

namespace Lib::UI {
class Container;
class Component;
class ILayoutManager {
public:
    explicit ILayoutManager() = default;
    virtual ~ILayoutManager() = default;

    virtual void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) = 0;
    virtual void layoutContainer(const std::shared_ptr<Container>& parent) = 0;
    virtual Math::Vector2 availableSizeFor(const std::shared_ptr<LayoutElement>& layoutElement) = 0;

private:
};
}