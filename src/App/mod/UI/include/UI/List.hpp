#pragma once
#include <Graphics/Color.hpp>
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
            return Math::Vector2({ 0, 0 });
        }
    };

    explicit List()
        : Container()
        , m_items()
        , m_cellRenderer()
        , m_selectedIndex(-1)
        , m_unselectColor({ 0.6f, 0.6f, 0.6f, 1.0f })
        , m_selectColor({ 0.6f, 0.8f, 0.6f, 1.0f })
    {
        setLayout(std::make_shared<Layout>());
    }

    void update()
    {
        if (!m_cellRenderer) {
            return;
        }
        auto inputSystem = Input::InputSystem::getInstance();
        auto mousePos = inputSystem->getMouse()->getPosition();
        auto mouseStatus = inputSystem->getMouse()->getState(Input::Mouse::Button::Left);

        auto screenPos = (((Math::Vector2)mousePos / (Math::Vector2)Graphics::Screen::getSize()) - Math::Vector2({ 0.5f, 0.5f })) * (Math::Vector2)Graphics::Screen::getSize();
        screenPos.y() *= -1;

        auto listCenter = getGlobalPosition();
        auto size = getSize();
        float top = listCenter.y() + (size.y() / 2.0f);
        float elementOffsetY = 0.0f;
        auto self = std::static_pointer_cast<List<T>>(shared_from_this());
        for (int32_t i = 0; i < static_cast<int32_t>(m_items.size()); i++) {
            const auto& item = m_items.at(i);
            bool isSelected = m_selectedIndex == i;
            auto comp = m_cellRenderer->getListCellRendererComponent(self, item, i, isSelected);
            auto prefSize = comp->getPreferredSize();

            float spaceOffsetY = (i + 1) * k_space;
            auto center = Math::Vector2({ listCenter.x(), top - (spaceOffsetY + elementOffsetY + (prefSize.y() / 2.0f)) });
            comp->setPosition(center);
            comp->setSize(prefSize);
            if (comp->isContains(screenPos)) {
                switch (mouseStatus) {
                case Input::ButtonState::None:
                    break;
                case Input::ButtonState::Pressed:
                    break;
                case Input::ButtonState::Released:
                    break;
                case Input::ButtonState::Trigger:
                    setSelectedIndex(i);
                    break;
                }
            }

            elementOffsetY += prefSize.y();
        }
    }

    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
    {
        if (!m_cellRenderer) {
            return;
        }
        auto listCenter = getGlobalPosition();
        auto size = getSize();
        float top = listCenter.y() + (size.y() / 2.0f);
        float elementOffsetY = 0.0f;
        auto self = std::static_pointer_cast<List<T>>(shared_from_this());
        for (int32_t i = 0; i < static_cast<int32_t>(m_items.size()); i++) {
            const auto& item = m_items.at(i);
            bool isSelected = m_selectedIndex == i;
            auto comp = m_cellRenderer->getListCellRendererComponent(self, item, i, isSelected);
            auto prefSize = comp->getPreferredSize();

            float spaceOffsetY = (i + 1) * k_space;
            auto center = Math::Vector2({ listCenter.x(), top - (spaceOffsetY + elementOffsetY + (prefSize.y() / 2.0f)) });
            if (i == m_selectedIndex) {
                renderer->drawRect(center, Math::Vector2({ size.x() - (k_space * 2), prefSize.y() }), 0.0f, m_selectColor);
            } else {
                renderer->drawRect(center, Math::Vector2({ size.x() - (k_space * 2), prefSize.y() }), 0.0f, m_unselectColor);
            }

            comp->setPosition(center);
            comp->setSize(prefSize);
            comp->draw2D(renderer);

            elementOffsetY += prefSize.y();
        }
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
    }

    std::shared_ptr<IListCellRenderer<T>> getCellRenderer() const
    {
        return m_cellRenderer;
    }

    void setSelectedIndex(int32_t selectedIndex)
    {
        m_selectedIndex = selectedIndex;
    }

    int32_t getSelectedIndex() const
    {
        return m_selectedIndex;
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
            return Math::Vector2({ maxWidth + (k_space * 2), totalHeight });
        }
        return Math::Vector2({ 0, 0 });
    }

private:
    static inline constexpr float k_space = 5.0f;
    std::vector<T> m_items;
    std::shared_ptr<IListCellRenderer<T>> m_cellRenderer;
    int32_t m_selectedIndex;
    Graphics::Color m_unselectColor;
    Graphics::Color m_selectColor;
};
}