#include <UI/Container.hpp>

namespace Lib::UI {

Container::Container()
    : Component()
    , m_children()
{
}
Container::~Container() { }

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
}