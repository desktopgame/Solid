#pragma once
#include <UI/Component.hpp>

namespace Lib::UI {
template <typename T>
class List;

template <typename T>
class IListCellRenderer {
public:
    IListCellRenderer() = default;
    virtual ~IListCellRenderer() = default;
    virtual std::shared_ptr<Component> getListCellRendererComponent(const std::shared_ptr<List<T>>& list, const T& item, int32_t index, bool isSelected) = 0;
};
}