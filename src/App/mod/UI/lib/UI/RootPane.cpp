#include <UI/BorderLayout.hpp>
#include <UI/RootPane.hpp>

namespace Lib::UI {
// Layout
void RootPane::Layout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
}
void RootPane::Layout::layoutContainer(const std::shared_ptr<Container>& parent)
{
    auto rootPane = std::static_pointer_cast<RootPane>(parent);
    auto contentPane = rootPane->getContentPane();
    contentPane->setPosition(Math::Vector2({ 0, 0 }));
}
Math::Vector2 RootPane::Layout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    return parent->getSize();
}
Math::Vector2 RootPane::Layout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    auto parentSize = parent->getSize();
    return Math::Vector2({ parentSize.x() - k_insets * 2,
        parentSize.y() - k_insets * 2 });
}
// public
std::shared_ptr<RootPane> RootPane::create()
{
    auto rootPane = std::shared_ptr<RootPane>(new RootPane());
    rootPane->initContentPane();
    return rootPane;
}

void RootPane::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    renderer->drawRect(getGlobalPosition(), getSize(), 0.0f, getBackgroundColor());
    Container::draw2D(renderer);
}

void RootPane::setLayout(const std::shared_ptr<ILayoutManager>& layoutManager) { m_contentPane->setLayout(layoutManager); }
std::shared_ptr<ILayoutManager> RootPane::getLayout() { return m_contentPane->getLayout(); }
void RootPane::addLayoutElement(const std::shared_ptr<LayoutElement>& layoutElement) { m_contentPane->addLayoutElement(layoutElement); }
void RootPane::removeLayoutElementAt(int32_t index) { m_contentPane->removeLayoutElementAt(index); }
std::shared_ptr<LayoutElement> RootPane::getLayoutElementAt(int32_t index) const { return m_contentPane->getLayoutElementAt(index); }
int32_t RootPane::getLayoutElementCount() const { return m_contentPane->getLayoutElementCount(); }

std::shared_ptr<Panel> RootPane::getContentPane() const { return m_contentPane; }
// private
RootPane::RootPane()
    : Container()
    , m_contentPane()
{
}

void RootPane::initContentPane()
{
    m_contentPane = std::make_shared<Panel>();
    m_contentPane->setLayout(std::make_shared<BorderLayout>());
    Container::addLayoutElement(std::make_shared<LayoutElement>(m_contentPane, nullptr));
    Container::setLayout(std::make_shared<RootPane::Layout>());
}
}