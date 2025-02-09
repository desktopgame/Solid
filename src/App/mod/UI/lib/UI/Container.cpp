#include <UI/Container.hpp>

namespace Lib::UI {

Container::Container()
    : Component()
    , m_children()
{
}
Container::~Container() { }

void Container::addChildren(const std::shared_ptr<Component>& child)
{
    m_children.emplace_back(child);
}
void Container::removeChildAt(int32_t index)
{
    m_children.erase(m_children.begin() + index);
}
std::shared_ptr<Component> Container::getChildAt(int32_t index) const
{
    return m_children.at(index);
}
int32_t Container::getChildCount() const
{
    return static_cast<int32_t>(m_children.size());
}
}