#pragma once
#include <UI/Container.hpp>
#include <UI/ILayoutManager.hpp>
#include <UI/LayoutHint.hpp>
#include <string>

namespace Lib::UI {
class TabbedPane : public Container {
public:
    class Layout : public ILayoutManager {
    public:
        void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) override;
        void layoutContainer(const std::shared_ptr<Container>& parent) override;
        Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent) override;
        Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint) override;
    };

    explicit TabbedPane();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setTitleAt(int32_t index, const std::u16string& title);
    std::u16string getTitleAt(int32_t index);

    void setSelectedIndex(int32_t selectedIndex);
    int32_t getSelectedIndex() const;

private:
    static inline constexpr float k_tabHeight = 50.0f;

    std::vector<std::u16string> m_titles;
    int32_t m_selectedIndex;
    int32_t m_hoverIndex;
};
}