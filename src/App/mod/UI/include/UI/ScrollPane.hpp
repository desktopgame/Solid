#pragma once
#include <UI/Component.hpp>
#include <UI/Container.hpp>

namespace Lib::UI {
class ScrollPane : public Container {
public:
    class Layout : public ILayoutManager {
    public:
        void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) override;
        void layoutContainer(const std::shared_ptr<Container>& parent) override;
        Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent) override;
        Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint) override;
    };

    explicit ScrollPane();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setView(const std::shared_ptr<Component>& view);
    std::shared_ptr<Component> getView() const;

    float getHorizontalScrollbarSize() const;
    float getVerticalScrollbarSize() const;

private:
    static inline constexpr float k_scrollBarSize = 10.0f;
    std::shared_ptr<Component> m_view;
    float m_horizontalScrollPosition;
    float m_verticalScrollPosition;
};
}