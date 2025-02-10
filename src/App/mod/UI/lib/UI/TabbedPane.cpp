#include <UI/TabbedPane.hpp>

namespace Lib::UI {
// Layout
void TabbedPane::Layout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
    Math::Vector2 minimumSize = parent->getMinimumSize();
    Math::Vector2 maximumSize = parent->getMaximumSize();
    parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
    parent->setPreferredSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
}
void TabbedPane::Layout::layoutContainer(const std::shared_ptr<Container>& parent)
{
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto e = parent->getLayoutElementAt(i);
        e->component->setPosition(Math::Vector2({ 0, -50 }));
    }
}
Math::Vector2 TabbedPane::Layout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    Math::Vector2 preferredSize;
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto e = parent->getLayoutElementAt(i);
        auto prefSize = e->component->getPreferredSizeSize();

        if (preferredSize.x() < prefSize.x()) {
            preferredSize.x() = prefSize.x();
        }
        if (preferredSize.y() < prefSize.y()) {
            preferredSize.y() = prefSize.y();
        }
    }
    return preferredSize;
}
Math::Vector2 TabbedPane::Layout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    auto availableSize = container->getPreferredSizeSize();
    if (Math::Mathf::equals(availableSize.x(), 0.0f) || Math::Mathf::equals(availableSize.y(), 0.0f)) {
        auto layout = container->getLayout();
        if (layout) {
            availableSize = layout->computePreferredSize(container);
        }
    }
    availableSize.y() = Math::Mathf::max(availableSize.y() - 50.0f, 0.0f);
    return availableSize;
}
// public
TabbedPane::TabbedPane()
    : Container()
    , m_titles()
{
    setLayout(std::make_shared<TabbedPane::Layout>());
}

void TabbedPane::update()
{
    if (getLayoutElementCount() > 0) {
        getLayoutElementAt(m_selectedIndex)->component->update();
    }
}
void TabbedPane::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getGlobalPosition(), getSize(), 0.0f, getBackgroundColor());

    if (getLayoutElementCount() > 0) {
        getLayoutElementAt(m_selectedIndex)->component->draw2D(renderer);
    }
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