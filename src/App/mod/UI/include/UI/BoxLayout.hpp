#pragma once
#include <UI/ILayoutManager.hpp>

namespace Lib::UI {
class BoxLayout : public ILayoutManager {
public:
    enum class Orientation {
        Horizontal,
        Vertical
    };

    explicit BoxLayout(Orientation orientation);
    void resizeContainer(const std::shared_ptr<Container>& parent);
    void layoutContainer(const std::shared_ptr<Container>& parent);

private:
    Orientation m_orientation;
};
}