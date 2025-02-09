#pragma once
#include <memory>

namespace Lib::UI {
class Container;
class Component;
class ILayoutManager {
public:
    explicit ILayoutManager() = default;
    virtual ~ILayoutManager() = default;

    virtual void layoutContainer(const std::shared_ptr<Container>& parent) = 0;

private:
};
}