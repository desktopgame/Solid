#pragma once
#include <UI/Component.hpp>
#include <UI/LayoutElement.hpp>
#include <memory>
#include <vector>

namespace Lib::UI {
class Container : public Component {
public:
    explicit Container();
    virtual ~Container();

    void addLayoutElement(const std::shared_ptr<LayoutElement>& layoutElement);
    void removeLayoutElementAt(int32_t index);
    std::shared_ptr<LayoutElement> getLayoutElementAt(int32_t index) const;
    int32_t getLayoutElementCount() const;

private:
    std::vector<std::shared_ptr<LayoutElement>> m_children;
};
}