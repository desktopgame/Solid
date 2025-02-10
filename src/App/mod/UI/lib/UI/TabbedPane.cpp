#include <UI/TabbedPane.hpp>

namespace Lib::UI {

TabbedPane::TabbedPane()
    : Container()
    , m_titles()
{
}

void TabbedPane::update() { }
void TabbedPane::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getGlobalPosition(), getSize(), 0.0f, getBackgroundColor());
}

void TabbedPane::setTitleAt(int32_t index, const std::u16string& title)
{
    m_titles.reserve(getLayoutElementCount());
    m_titles.at(index) = title;
}
std::u16string TabbedPane::getTitleAt(int32_t index) { return m_titles.at(index); }

void TabbedPane::setSelectedIndex(int32_t selectedIndex) { m_selectedIndex = selectedIndex; }
int32_t TabbedPane::getSelectedIndex() const { return m_selectedIndex; }
}