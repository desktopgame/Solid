#pragma once
#include <UI/Container.hpp>
#include <UI/ILayoutManager.hpp>
#include <UI/Panel.hpp>

namespace Lib::UI {
class RootPane : public Container {
public:
    class Layout : public ILayoutManager {
    public:
        void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) override;
        void layoutContainer(const std::shared_ptr<Container>& parent) override;
        Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent) override;
        Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint) override;
    };
    static std::shared_ptr<RootPane> create();

    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setLayout(const std::shared_ptr<ILayoutManager>& layoutManager) override;
    std::shared_ptr<ILayoutManager> getLayout() override;
    void addLayoutElement(const std::shared_ptr<LayoutElement>& layoutElement) override;
    void removeLayoutElementAt(int32_t index) override;
    std::shared_ptr<LayoutElement> getLayoutElementAt(int32_t index) const override;
    int32_t getLayoutElementCount() const override;

    std::shared_ptr<Panel> getContentPane() const;

private:
    RootPane();
    void initContentPane();

    static inline constexpr float k_insets = 5.0f;
    std::shared_ptr<Panel> m_contentPane;
};
}