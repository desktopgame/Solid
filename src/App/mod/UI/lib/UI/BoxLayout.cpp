#include <UI/BoxLayout.hpp>

namespace Lib::UI {

BoxLayout::BoxLayout(BoxLayout::Orientation orientation)
    : ILayoutManager()
    , m_orientation(orientation)
{
    (void)m_orientation;
}
void BoxLayout::resizeContainer(const std::shared_ptr<Container>& parent)
{
}
void BoxLayout::layoutContainer(const std::shared_ptr<Container>& parent)
{
}
}