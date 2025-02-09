#include <UI/Container.hpp>

namespace Lib::UI {

Container::Container()
    : Component()
    , m_layoutManager()
    , m_children()
{
}
Container::~Container() { }

void Container::update()
{
    for (const auto& c : m_children) {
        c->component->update();
    }
}
void Container::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    for (const auto& c : m_children) {
        c->component->draw2D(renderer);
    }
}

void Container::doLayout()
{
    layoutTopdown(getSize());
    layoutBottomup();
}

void Container::setLayout(const std::shared_ptr<ILayoutManager>& layoutManager)
{
    m_layoutManager = layoutManager;
}
std::shared_ptr<ILayoutManager> Container::getLayout()
{
    return m_layoutManager;
}

void Container::addLayoutElement(const std::shared_ptr<LayoutElement>& child)
{
    m_children.emplace_back(child);
}
void Container::removeLayoutElementAt(int32_t index)
{
    m_children.erase(m_children.begin() + index);
}
std::shared_ptr<LayoutElement> Container::getLayoutElementAt(int32_t index) const
{
    return m_children.at(index);
}
int32_t Container::getLayoutElementCount() const
{
    return static_cast<int32_t>(m_children.size());
}
// private
void Container::layoutTopdown(const Math::Vector2& availableSize)
{
    if (m_layoutManager) {
        auto self = std::static_pointer_cast<Container>(shared_from_this());
        if (getParent()) {
            m_layoutManager->resizeContainer(self, availableSize);
        }

        for (auto c : m_children) {
            auto container = std::dynamic_pointer_cast<Container>(c->component);
            if (container) {
                container->layoutTopdown(m_layoutManager->availableSizeFor(self, c));
            }
        }
    }
}
void Container::layoutBottomup()
{
    if (m_layoutManager) {
        for (auto c : m_children) {
            auto container = std::dynamic_pointer_cast<Container>(c->component);
            if (container) {
                container->layoutBottomup();
            }
        }

        auto self = std::static_pointer_cast<Container>(shared_from_this());
        m_layoutManager->layoutContainer(self);
    }
}
}