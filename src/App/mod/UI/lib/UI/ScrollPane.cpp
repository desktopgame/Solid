#include <Graphics/Screen.hpp>
#include <Input/InputSystem.hpp>
#include <Input/Mouse.hpp>
#include <Math/Mathf.hpp>
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
    auto viewSize = view->getPreferredSize();
    float hPos = scrollPane->getHorizontalScrollPosition();
    float vPos = scrollPane->getVerticalScrollPosition();

    view->setSize(viewSize);

    float scrollOffsetX = hPos * (viewSize.x() - scrollPane->getSize().x() + k_scrollBarSize);
    float scrollOffsetY = vPos * (viewSize.y() - scrollPane->getSize().y() + k_scrollBarSize);

    float baseOffsetX = -(scrollPane->getSize().x() / 2.0f) + (viewSize.x() / 2.0f);
    float baseOffsetY = (scrollPane->getSize().y() / 2.0f) - (viewSize.y() / 2.0f);

    if (viewSize.x() < scrollPane->getSize().x()) {
        baseOffsetX = 0;
    }
    if (viewSize.y() < scrollPane->getSize().y()) {
        baseOffsetY = 0;
    }
    view->setPosition(Math::Vector2({ //
        baseOffsetX - scrollOffsetX,
        baseOffsetY + scrollOffsetY }));
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
    auto availableSize = container->getPreferredSize();
    if (Math::Mathf::equals(availableSize.x(), 0.0f) || Math::Mathf::equals(availableSize.y(), 0.0f)) {
        auto layout = container->getLayout();
        if (layout) {
            availableSize = layout->computePreferredSize(container);
        }
    }
    return availableSize;
}
// public
ScrollPane::ScrollPane()
    : Container()
    , m_view()
    , m_horizontalScrollPosition()
    , m_verticalScrollPosition()
    , m_status()
    , m_hWarp()
    , m_vWarp()
{
    setLayout(std::make_shared<ScrollPane::Layout>());
}

void ScrollPane::update()
{
    auto inputSystem = Input::InputSystem::getInstance();
    auto mousePos = inputSystem->getMouse()->getPosition();
    auto mouseStatus = inputSystem->getMouse()->getState(Input::Mouse::Button::Left);

    auto screenPos = (((Math::Vector2)mousePos / (Math::Vector2)Graphics::Screen::getSize()) - Math::Vector2({ 0.5f, 0.5f })) * (Math::Vector2)Graphics::Screen::getSize();
    screenPos.y() *= -1;

    auto center = getGlobalPosition();
    auto size = getSize();
    auto viewportSize = size - Math::Vector2({ k_scrollBarSize, k_scrollBarSize });
    float horizontalScrollbarSize = getHorizontalScrollbarSize();
    float verticalScrollbarSize = getVerticalScrollbarSize();
    bool scrollChanged = false;

    Math::Vector2 horizontalScrollbarOffset = getHorizontalScrollbarOffset();
    Math::Vector2 verticalScrollbarOffset = getVerticalScrollbarOffset();

    float horizontalScrollbarLeft = center.x() + horizontalScrollbarOffset.x() - (horizontalScrollbarSize / 2.0f);
    float horizontalScrollbarRight = center.x() + horizontalScrollbarOffset.x() + (horizontalScrollbarSize / 2.0f);
    float horizontalScrollbarTop = center.y() + horizontalScrollbarOffset.y() + (k_scrollBarSize / 2.0f);
    float horizontalScrollbarBottom = center.y() + horizontalScrollbarOffset.y() - (k_scrollBarSize / 2.0f);

    bool hoverHorizontalScrollbar = true;
    if (screenPos.x() < horizontalScrollbarLeft || screenPos.x() > horizontalScrollbarRight) {
        hoverHorizontalScrollbar = false;
    }
    if (screenPos.y() < horizontalScrollbarBottom || screenPos.y() > horizontalScrollbarTop) {
        hoverHorizontalScrollbar = false;
    }

    if (hoverHorizontalScrollbar) {
        m_status = k_scrollbarHover;

        if (mouseStatus == Input::ButtonState::Trigger) {
            m_hWarp = true;
        }
        if (mouseStatus == Input::ButtonState::Pressed && m_hWarp) {
            m_status = k_scrollbarActive;
            float delta = -((center.x() + horizontalScrollbarOffset.x() - screenPos.x()) / size.x());
            if (!Math::Mathf::equals(horizontalScrollbarSize, viewportSize.x())) {
                setHorizontalScrollPosition(getHorizontalScrollPosition() + delta);
            }
            scrollChanged = true;
        }
    } else if (m_hWarp) {
        if (mouseStatus == Input::ButtonState::Pressed) {
            m_status = k_scrollbarActive;
            float delta = -((center.x() + horizontalScrollbarOffset.x() - screenPos.x()) / size.x());
            if (!Math::Mathf::equals(horizontalScrollbarSize, viewportSize.x())) {
                setHorizontalScrollPosition(getHorizontalScrollPosition() + delta);
            }
            scrollChanged = true;
        }
    }

    float verticalScrollbarLeft = center.x() + verticalScrollbarOffset.x() - (k_scrollBarSize / 2.0f);
    float verticalScrollbarRight = center.x() + verticalScrollbarOffset.x() + (k_scrollBarSize / 2.0f);
    float verticalScrollbarTop = center.y() + verticalScrollbarOffset.y() + (verticalScrollbarSize / 2.0f);
    float verticalScrollbarBottom = center.y() + verticalScrollbarOffset.y() - (verticalScrollbarSize / 2.0f);

    bool hoverVerticalScrollbar = true;
    if (screenPos.x() < verticalScrollbarLeft || screenPos.x() > verticalScrollbarRight) {
        hoverVerticalScrollbar = false;
    }
    if (screenPos.y() < verticalScrollbarBottom || screenPos.y() > verticalScrollbarTop) {
        hoverVerticalScrollbar = false;
    }

    if (hoverVerticalScrollbar) {
        m_status = k_scrollbarHover;

        if (mouseStatus == Input::ButtonState::Trigger) {
            m_vWarp = true;
        }
        if (mouseStatus == Input::ButtonState::Pressed && m_vWarp) {
            m_status = k_scrollbarActive;
            float delta = ((center.y() + verticalScrollbarOffset.y() - screenPos.y()) / size.y());
            if (!Math::Mathf::equals(verticalScrollbarSize, viewportSize.y())) {
                setVerticalScrollPosition(getVerticalScrollPosition() + delta);
            }
            scrollChanged = true;
        }
    } else if (m_vWarp) {
        if (mouseStatus == Input::ButtonState::Pressed) {
            m_status = k_scrollbarActive;
            float delta = ((center.y() + verticalScrollbarOffset.y() - screenPos.y()) / size.y());
            if (!Math::Mathf::equals(verticalScrollbarSize, viewportSize.y())) {
                setVerticalScrollPosition(getVerticalScrollPosition() + delta);
            }
            scrollChanged = true;
        }
    }

    if (scrollChanged) {
        doLayout();
    }

    if (mouseStatus == Input::ButtonState::Released) {
        m_hWarp = false;
        m_vWarp = false;
    }

    m_view->update();
}
void ScrollPane::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    auto center = getGlobalPosition();
    auto size = getSize();
    auto viewSize = m_view->getSize();
    auto viewportSize = size - Math::Vector2({ k_scrollBarSize, k_scrollBarSize });
    renderer->stencilClear();
    renderer->stencilWrite();
    renderer->stencilRef(1);
    renderer->drawRect(center, size, 0.0f, getBackgroundColor());

    renderer->stencilRead();
    m_view->draw2D(renderer);

    renderer->stencilNone();

    // scrollbar
    float horizontalScrollbarSize = getHorizontalScrollbarSize();
    float verticalScrollbarSize = getVerticalScrollbarSize();
    (void)horizontalScrollbarSize;
    (void)verticalScrollbarSize;
    (void)m_horizontalScrollPosition;
    (void)m_verticalScrollPosition;
    (void)m_horizontalScrollPosition;
    (void)viewSize;
    (void)viewportSize;
    renderer->drawRect(center + Math::Vector2({ (size.x() / 2.0f) - (k_scrollBarSize / 2.0f), 0 }), Math::Vector2({ k_scrollBarSize, size.y() }), 0.0f, Math::Vector4({ 0.2f, 0.2f, 0.6f, 1.0f }));
    renderer->drawRect(center - Math::Vector2({ 0, (size.y() / 2.0f) - (k_scrollBarSize / 2.0f) }), Math::Vector2({ size.x(), k_scrollBarSize }), 0.0f, Math::Vector4({ 0.2f, 0.2f, 0.6f, 1.0f }));

    renderer->drawRect(center + getVerticalScrollbarOffset(), Math::Vector2({ k_scrollBarSize, verticalScrollbarSize }), 0.0f, Math::Vector4({ 0.4f, 0.4f, 0.6f, 1.0f }));
    renderer->drawRect(center + getHorizontalScrollbarOffset(), Math::Vector2({ horizontalScrollbarSize, k_scrollBarSize }), 0.0f, Math::Vector4({ 0.4f, 0.4f, 0.6f, 1.0f }));
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

void ScrollPane::setHorizontalScrollPosition(float horizontalScrollPosition) { m_horizontalScrollPosition = Math::Mathf::clamp(0.0f, 1.0f, horizontalScrollPosition); }
float ScrollPane::getHorizontalScrollPosition() const { return m_horizontalScrollPosition; }

void ScrollPane::setVerticalScrollPosition(float verticalScrollPosition) { m_verticalScrollPosition = Math::Mathf::clamp(0.0f, 1.0f, verticalScrollPosition); }
float ScrollPane::getVerticalScrollPosition() const { return m_verticalScrollPosition; }

float ScrollPane::getHorizontalScrollbarSize() const
{
    auto size = getSize();
    auto viewSize = m_view->getSize();
    auto viewportSize = size - Math::Vector2({ k_scrollBarSize, k_scrollBarSize });
    float horizontalScrollbarSize = (viewportSize.x() / viewSize.x()) * viewportSize.x();
    if (viewSize.x() < viewportSize.x()) {
        horizontalScrollbarSize = viewportSize.x();
    }
    return horizontalScrollbarSize;
}
float ScrollPane::getVerticalScrollbarSize() const
{
    auto size = getSize();
    auto viewSize = m_view->getSize();
    auto viewportSize = size - Math::Vector2({ k_scrollBarSize, k_scrollBarSize });
    float verticalScrollbarSize = (viewportSize.y() / viewSize.y()) * viewportSize.y();
    if (viewSize.y() < viewportSize.y()) {
        verticalScrollbarSize = viewportSize.y();
    }
    return verticalScrollbarSize;
}
// private
Math::Vector2 ScrollPane::getHorizontalScrollbarOffset() const
{
    auto size = getSize();
    float horizontalScrollbarSize = getHorizontalScrollbarSize();
    Math::Vector2 horizontalScrollbarOffset = Math::Vector2({ //
        -(size.x() / 2.0f) + (horizontalScrollbarSize / 2.0f) + ((size.x() - horizontalScrollbarSize - k_scrollBarSize)) * m_horizontalScrollPosition,
        -(size.y() / 2.0f) + (k_scrollBarSize / 2.0f) });
    return horizontalScrollbarOffset;
}
Math::Vector2 ScrollPane::getVerticalScrollbarOffset() const
{
    auto size = getSize();
    float verticalScrollbarSize = getVerticalScrollbarSize();
    Math::Vector2 verticalScrollbarOffset = Math::Vector2({ //
        (size.x() / 2.0f) - (k_scrollBarSize / 2.0f),
        (size.y() / 2.0f) - (verticalScrollbarSize / 2.0f) - ((size.y() - verticalScrollbarSize - k_scrollBarSize)) * m_verticalScrollPosition });
    return verticalScrollbarOffset;
}
}