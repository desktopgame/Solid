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

    void setHorizontalScrollPosition(float horizontalScrollPosition);
    float getHorizontalScrollPosition() const;

    void setVerticalScrollPosition(float verticalScrollPosition);
    float getVerticalScrollPosition() const;

    float getHorizontalScrollbarSize() const;
    float getVerticalScrollbarSize() const;

private:
    Math::Vector2 getHorizontalScrollbarOffset() const;
    Math::Vector2 getVerticalScrollbarOffset() const;

    static inline constexpr float k_scrollBarSize = 10.0f;
    static inline constexpr int32_t k_scrollbarNone = 0;
    static inline constexpr int32_t k_scrollbarHover = 1;
    static inline constexpr int32_t k_scrollbarActive = 2;

    std::shared_ptr<Component> m_view;
    float m_horizontalScrollPosition;
    float m_verticalScrollPosition;
    int32_t m_status;
    bool m_hWarp;
    bool m_vWarp;
};
}