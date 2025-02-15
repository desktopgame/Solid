#pragma once
#include <UI/IListCellRenderer.hpp>
#include <UI/Label.hpp>

namespace Lib::UI {
class StringListCellRenderer : public Label, public IListCellRenderer<std::u16string> {
public:
    explicit StringListCellRenderer();
    std::shared_ptr<Component> getListCellRendererComponent(const std::shared_ptr<List<std::u16string>>& list, const std::u16string& item, int32_t index, bool isSelected) override;

private:
};
}