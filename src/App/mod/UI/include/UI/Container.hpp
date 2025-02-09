#pragma once
#include <UI/Component.hpp>
#include <memory>
#include <vector>

namespace Lib::UI {
class Container : public Component {
public:
    explicit Container();
    virtual ~Container();

    void addChildren(const std::shared_ptr<Component>& child);
    void removeChildAt(int32_t index);
    std::shared_ptr<Component> getChildAt(int32_t index) const;
    int32_t getChildCount() const;

private:
    std::vector<std::shared_ptr<Component>> m_children;
};
}