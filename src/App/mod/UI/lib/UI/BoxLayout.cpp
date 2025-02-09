#include <UI/BoxLayout.hpp>

namespace Lib::UI {

BoxLayout::BoxLayout(BoxLayout::Orientation orientation)
    : ILayoutManager()
    , m_orientation(orientation)
{
}
void BoxLayout::resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize)
{
    switch (m_orientation) {
    case BoxLayout::Orientation::Horizontal:
        break;
    case BoxLayout::Orientation::Vertical:
        break;
    }
}
void BoxLayout::layoutContainer(const std::shared_ptr<Container>& parent)
{
}
}