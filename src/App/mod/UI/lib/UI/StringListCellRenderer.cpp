#include <UI/StringListCellRenderer.hpp>

namespace Lib::UI {
StringListCellRenderer::StringListCellRenderer()
    : Label()
{
}
std::shared_ptr<Component> StringListCellRenderer::getListCellRendererComponent(const std::shared_ptr<List<std::u16string>>& list, const std::u16string& item, int32_t index, bool isSelected)
{
    auto self = shared_from_this();
    setText(item);
    return self;
}
}