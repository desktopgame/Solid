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
    auto parentSize = parent->getSize();
    for (int32_t i = 0; i < parent->getLayoutElementCount(); i++) {
        auto e = parent->getLayoutElementAt(i);
        auto prefSize = e->component->getPreferredSizeSize();
        auto maxSize = e->component->getMaximumSize();
        if (e->component->isFlexible()) {
            prefSize.x() = Math::Mathf::min(parentSize.x(), maxSize.x());
            prefSize.y() = Math::Mathf::min(parentSize.y() - 50.0f, maxSize.y());
        }
        e->component->setSize(prefSize);
        e->component->setPosition(Math::Vector2({ 0, -((parentSize.y()) / 4.0f) + 50.0f }));
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
    , m_selectedIndex(0)
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
    auto center = getGlobalPosition();
    renderer->drawRect(center, getSize(), 0.0f, getBackgroundColor());
    renderer->drawRect(center + Math::Vector2({ 0, (getSize().y() / 2.0f) - 25.0f }), Math::Vector2({ getSize().x(), 50.0f }), 0.0f, Math::Vector4({ 0.5f, 0.5f, 0.5f, 1.0f }));

    float tabOffset = -(getSize().x() / 2.0f) + 50.0f;
    for (int32_t i = 0; i < getLayoutElementCount(); i++) {
        std::string title = std::to_string(i);
        std::u16string uTitle = std::u16string(title.begin(), title.end());
        if (i < m_titles.size()) {
            uTitle = m_titles.at(i);
        }
        renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - 25.0f }), Math::Vector2({ 100.0f, 50.0f }), 0.0f, Math::Vector4({ 1, 0, 0, 1 }));
    }

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