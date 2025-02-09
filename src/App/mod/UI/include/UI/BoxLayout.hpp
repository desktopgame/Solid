#pragma once
#include <UI/ILayoutManager.hpp>

namespace Lib::UI {
class BoxLayout : public ILayoutManager {
public:
    enum class Orientation {
        Horizontal,
        Vertical
    };

    static inline constexpr float k_space = 5.0f;

    explicit BoxLayout(Orientation orientation);
    void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize);
    void layoutContainer(const std::shared_ptr<Container>& parent);
    Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent);
    Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container);

private:
    Orientation m_orientation;
};
}