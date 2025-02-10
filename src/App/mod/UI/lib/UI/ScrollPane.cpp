#include <UI/ScrollPane.hpp>
#include <stdexcept>

namespace Lib::UI {
// Layout
void ScrollPane::Layout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
    Math::Vector2 minimumSize = parent->getMinimumSize();
    Math::Vector2 maximumSize = parent->getMaximumSize();
    parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
    parent->setPreferredSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
        Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
}
void ScrollPane::Layout::layoutContainer(const std::shared_ptr<Container>& parent)
{
    auto scrollPane = std::static_pointer_cast<ScrollPane>(parent);
    auto view = scrollPane->getView();

    if (view->isFlexible()) {
        view->setSize(parent->getSize() - Math::Vector2({ k_scrollBarSize, k_scrollBarSize }));
    } else {
        view->setSize(view->getPreferredSize());
    }
    view->setPosition(Math::Vector2({ -k_scrollBarSize / 2.0f, k_scrollBarSize / 2.0f }));
}
Math::Vector2 ScrollPane::Layout::computePreferredSize(const std::shared_ptr<Container>& parent)
{
    auto scrollPane = std::static_pointer_cast<ScrollPane>(parent);
    auto view = scrollPane->getView();
    auto viewContainer = std::dynamic_pointer_cast<Container>(view);
    if (viewContainer) {
        auto layout = viewContainer->getLayout();
        if (layout) {
            return layout->computePreferredSize(viewContainer);
        }
    }
    return view->getPreferredSize();
}
Math::Vector2 ScrollPane::Layout::availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint)
{
    return parent->getSize() - Math::Vector2({ k_scrollBarSize, k_scrollBarSize });
}
// public
ScrollPane::ScrollPane()
    : Container()
    , m_view()
{
    setLayout(std::make_shared<ScrollPane::Layout>());
}

void ScrollPane::update()
{
    m_view->update();
}
void ScrollPane::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    auto size = getSize();
    renderer->stencilClear();
    renderer->stencilWrite();
    renderer->stencilRef(1);
    renderer->drawRect(center, size, 0.0f, getBackgroundColor());

    renderer->stencilRead();
    m_view->draw2D(renderer);

    renderer->stencilNone();

    // scrollbar
    renderer->drawRect(center + Math::Vector2({ (size.x() / 2.0f) - (k_scrollBarSize / 2.0f), 0 }), Math::Vector2({ k_scrollBarSize, size.y() }), 0.0f, getBackgroundColor());
    renderer->drawRect(center - Math::Vector2({ 0, (size.y() / 2.0f) - (k_scrollBarSize / 2.0f) }), Math::Vector2({ size.x(), k_scrollBarSize }), 0.0f, getBackgroundColor());
}

void ScrollPane::setView(const std::shared_ptr<Component>& view)
{
    if (m_view) {
        throw std::logic_error("view is already set.");
    }
    m_view = view;
    addLayoutElement(std::make_shared<LayoutElement>(view, nullptr));
}
std::shared_ptr<Component> ScrollPane::getView() const { return m_view; }
}