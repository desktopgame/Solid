#pragma once
#include <UI/Container.hpp>
#include <UI/ILayoutManager.hpp>
#include <UI/IListCellRenderer.hpp>
#include <vector>

namespace Lib::UI {
template <typename T>
class List : public Container {
public:
    class Layout : public ILayoutManager {
    public:
        void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) override
        {
            Math::Vector2 minimumSize = parent->getMinimumSize();
            Math::Vector2 maximumSize = parent->getMaximumSize();
            parent->setSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
                Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
            parent->setPreferredSize(Math::Vector2({ Math::Mathf::clamp(minimumSize.x(), maximumSize.x(), availableSize.x()),
                Math::Mathf::clamp(minimumSize.y(), maximumSize.y(), availableSize.y()) }));
        }
        void layoutContainer(const std::shared_ptr<Container>& parent) override { }
        Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent) override
        {
            auto list = std::static_pointer_cast<List<T>>(parent);
            return list->computePreferredSize();
        }
        Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint) override
        {
            return Vector2({ 0, 0 });
        }
    };

    explicit List()
        : Container()
        , m_items()
        , m_cellRenderer()
        , m_selectedIndex(-1)
    {
        setLayout(std::make_shared<Layout>());
    }

    void update() { }

    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
    {
        if (!m_cellRenderer) {
            return;
        }
        auto center = getGlobalPosition();
        auto size = getSize();
        float left = center.x() - (size.x() / 2.0f);
        float top = center.y() + (size.y() / 2.0f);
        float elementOffsetY = 0.0f;
        auto self = std::static_pointer_cast<List<T>>(shared_from_this());
        for (int32_t i = 0; i < static_cast<int32_t>(m_items.size()); i++) {
            const auto& item = m_items.at(i);
            bool isSelected = m_selectedIndex == i;
            auto comp = m_cellRenderer->getListCellRendererComponent(self, item, i, isSelected);
            auto prefSize = comp->getPreferredSize();

            float spaceOffsetX = k_space;
            float spaceOffsetY = (i + 1) * k_space;
            comp->setPosition(Vector2({ left + spaceOffsetX, top - (spaceOffsetY + elementOffsetY + (prefSize.y() / 2.0f)) }));
            comp->setSize(prefSize);
            comp->draw2D(renderer);

            elementOffsetY += prefSize.y();
        }
    }

    void modifyItems()
    {
        calculateSize();
    }

    void addItem(const T& item)
    {
        m_items.emplace_back(item);
    }

    T& getItemAt(int32_t index)
    {
        return m_items.at(index);
    }

    const T& getItemAt(int32_t index) const
    {
        return m_items.at(index);
    }

    void removeItemAt(int32_t index)
    {
        m_items.erase(m_items.begin() + index);
    }

    int32_t getItemCount() const
    {
        return static_cast<int32_t>(m_items.size());
    }

    void setCellRenderer(const std::shared_ptr<IListCellRenderer<T>>& cellRenderer)
    {
        m_cellRenderer = cellRenderer;
        calculateSize();
    }

    std::shared_ptr<IListCellRenderer<T>> getCellRenderer() const
    {
        return m_cellRenderer;
    }

    Math::Vector2 computePreferredSize()
    {
        if (m_cellRenderer) {
            auto self = std::static_pointer_cast<List<T>>(shared_from_this());
            float maxWidth = -999;
            float totalHeight = 0.0f;
            for (int32_t i = 0; i < static_cast<int32_t>(m_items.size()); i++) {
                const auto& item = m_items.at(i);
                bool isSelected = m_selectedIndex == i;
                auto comp = m_cellRenderer->getListCellRendererComponent(self, item, i, isSelected);
                auto prefSize = comp->getPreferredSize();

                if (prefSize.x() > maxWidth) {
                    maxWidth = prefSize.x();
                }
                totalHeight += prefSize.y();
            }
            totalHeight += (m_items.size() + 1) * k_space;
            return Vector2({ maxWidth, totalHeight });
        }
        return Vector2({ 0, 0 });
    }

private:
    static inline constexpr float k_space = 5.0f;
    std::vector<T> m_items;
    std::shared_ptr<IListCellRenderer<T>> m_cellRenderer;
    int32_t m_selectedIndex;
};
}