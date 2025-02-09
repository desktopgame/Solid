#pragma once
#include <UI/Component.hpp>
#include <UI/ILayoutManager.hpp>
#include <UI/LayoutElement.hpp>
#include <memory>
#include <vector>

namespace Lib::UI {
class Container : public Component {
public:
    explicit Container();
    virtual ~Container();

    virtual void update() override;
    virtual void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void doLayout();

    void setLayout(const std::shared_ptr<ILayoutManager>& layoutManager);
    std::shared_ptr<ILayoutManager> getLayout();

    void addLayoutElement(const std::shared_ptr<LayoutElement>& layoutElement);
    void removeLayoutElementAt(int32_t index);
    std::shared_ptr<LayoutElement> getLayoutElementAt(int32_t index) const;
    int32_t getLayoutElementCount() const;

private:
    void layoutTopdown(const Math::Vector2& availableSize);
    void layoutBottomup();

    std::shared_ptr<ILayoutManager> m_layoutManager;
    std::vector<std::shared_ptr<LayoutElement>> m_children;
};
}