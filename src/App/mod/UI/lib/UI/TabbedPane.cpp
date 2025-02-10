#include <Graphics/Screen.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Mouse.hpp>
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
            prefSize.y() = Math::Mathf::min(parentSize.y() - k_tabHeight, maxSize.y());
        }
        e->component->setSize(prefSize);
        e->component->setPosition(Math::Vector2({ 0, -((parentSize.y()) / 4.0f) + k_tabHeight }));
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
    auto parentSize = parent->getSize();
    return Math::Vector2({ parentSize.x(),
        parentSize.y() - k_tabHeight });
}
// public
TabbedPane::TabbedPane()
    : Container()
    , m_titles()
    , m_selectedIndex(0)
    , m_hoverIndex(-1)
{
    setLayout(std::make_shared<TabbedPane::Layout>());
}

void TabbedPane::update()
{
    auto inputSystem = Input::InputSystem::getInstance();
    auto mousePos = inputSystem->getMouse()->getPosition();
    auto mouseStatus = inputSystem->getMouse()->getState(Input::Mouse::Button::Left);

    auto screenPos = (((Math::Vector2)mousePos / (Math::Vector2)Graphics::Screen::getSize()) - Math::Vector2({ 0.5f, 0.5f })) * (Math::Vector2)Graphics::Screen::getSize();
    screenPos.y() *= -1;

    m_hoverIndex = -1;
    auto center = getGlobalPosition();
    float k_tabWidth = getSize().x() / static_cast<float>(getLayoutElementCount());
    float tabOffset = center.x() - (getSize().x() / 2.0f) + (k_tabWidth / 2.0f);
    for (int32_t i = 0; i < getLayoutElementCount(); i++) {
        std::string title = std::to_string(i);
        std::u16string uTitle = std::u16string(title.begin(), title.end());
        if (i < m_titles.size()) {
            uTitle = m_titles.at(i);
        }
        Math::Vector2 tabCenter = Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) });
        Math::Vector2 tabSize = Math::Vector2({ k_tabWidth, k_tabHeight });
        float tabLeft = tabCenter.x() - (tabSize.x() / 2.0f);
        float tabRight = tabCenter.x() + (tabSize.x() / 2.0f);
        float tabBottom = tabCenter.y() - (tabSize.y() / 2.0f);
        float tabTop = tabCenter.y() + (tabSize.y() / 2.0f);

        bool tabHover = true;
        if (screenPos.x() < tabLeft || screenPos.x() > tabRight) {
            tabHover = false;
        }
        if (screenPos.y() < tabBottom || screenPos.y() > tabTop) {
            tabHover = false;
        }

        if (tabHover) {
            m_hoverIndex = i;

            if (mouseStatus == Input::ButtonState::Trigger) {
                m_selectedIndex = i;
                break;
            }
        }
        tabOffset += k_tabWidth;
    }

    if (getLayoutElementCount() > 0) {
        getLayoutElementAt(m_selectedIndex)->component->update();
    }
}
void TabbedPane::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    renderer->drawRect(center, getSize(), 0.0f, getBackgroundColor());
    renderer->drawRect(center + Math::Vector2({ 0, (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ getSize().x(), k_tabHeight }), 0.0f, Math::Vector4({ 0.5f, 0.5f, 0.5f, 1.0f }));

    renderer->textFont(getFont());
    renderer->textFontSize(getFontSize());

    float k_tabWidth = getSize().x() / static_cast<float>(getLayoutElementCount());
    float tabOffset = center.x() - (getSize().x() / 2.0f) + (k_tabWidth / 2.0f);
    for (int32_t i = 0; i < getLayoutElementCount(); i++) {
        std::string title = std::to_string(i);
        std::u16string uTitle = std::u16string(title.begin(), title.end());
        if (i < m_titles.size()) {
            uTitle = m_titles.at(i);
        }
        if (i == m_selectedIndex) {
            renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ k_tabWidth, k_tabHeight }), 0.0f, Graphics::Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
            renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ k_tabWidth - 2.0f, k_tabHeight - 2.0f }), 0.0f, Graphics::Color({ 0.2f, 0.6f, 0.2f, 1.0f }));
        } else if (i == m_hoverIndex) {
            renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ k_tabWidth, k_tabHeight }), 0.0f, Graphics::Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
            renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ k_tabWidth - 2.0f, k_tabHeight - 2.0f }), 0.0f, Graphics::Color({ 0.4f, 0.6f, 0.4f, 1.0f }));
        } else {
            renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ k_tabWidth, k_tabHeight }), 0.0f, Graphics::Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
            renderer->drawRect(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Math::Vector2({ k_tabWidth - 2.0f, k_tabHeight - 2.0f }), 0.0f, Graphics::Color({ 0.4f, 0.4f, 0.4f, 1.0f }));
        }
        renderer->drawText(Math::Vector2({ tabOffset, center.y() + (getSize().y() / 2.0f) - (k_tabHeight / 2.0f) }), Graphics::Renderer::TextAlignX::Center, Graphics::Renderer::TextAlignY::Center, uTitle, getForegroundColor());
        tabOffset += k_tabWidth;
    }

    if (getLayoutElementCount() > 0) {
        getLayoutElementAt(m_selectedIndex)->component->draw2D(renderer);
    }
}

void TabbedPane::setTitleAt(int32_t index, const std::u16string& title)
{
    m_titles.resize(getLayoutElementCount());
    m_titles.at(index) = title;
}
std::u16string TabbedPane::getTitleAt(int32_t index) { return m_titles.at(index); }

void TabbedPane::setSelectedIndex(int32_t selectedIndex) { m_selectedIndex = selectedIndex; }
int32_t TabbedPane::getSelectedIndex() const { return m_selectedIndex; }
}