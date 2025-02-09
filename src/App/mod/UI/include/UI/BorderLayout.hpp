#pragma once
#include <UI/ILayoutManager.hpp>
#include <UI/LayoutHint.hpp>

namespace Lib::UI {
class BorderLayout : public ILayoutManager {
public:
    class Hint : public LayoutHint {
    public:
        std::string location;
    };

    explicit BorderLayout() = default;
    void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) override;
    void layoutContainer(const std::shared_ptr<Container>& parent) override;
    Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent) override;
    Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint) override;

private:
    bool findLocation(const std::shared_ptr<Container>& parent, const std::string& location);
};
}