#pragma once
#include <UI/Container.hpp>
#include <UI/LayoutHint.hpp>
#include <string>

namespace Lib::UI {
class TabbedPane : public Container {
public:
    explicit TabbedPane();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setTitleAt(int32_t index, const std::u16string& title);
    std::u16string getTitleAt(int32_t index);

    void setSelectedIndex(int32_t selectedIndex);
    int32_t getSelectedIndex() const;

private:
    std::vector<std::u16string> m_titles;
    int32_t m_selectedIndex;
};
}