#pragma once
#include <Graphics/FontMap.hpp>
#include <Graphics/Renderer.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::UI {
class Container;
class Component : public std::enable_shared_from_this<Component> {
public:
    explicit Component();
    virtual ~Component();

    virtual void update() = 0;
    virtual void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) = 0;

    void setParent(const std::shared_ptr<Container>& parent);
    std::shared_ptr<Container> getParent() const;

    void setSize(const Math::Vector2& size);
    Math::Vector2 getSize() const;

    void setPosition(const Math::Vector2& position);
    Math::Vector2 getPosition() const;

    Math::Vector2 getGlobalPosition() const;

    void setMinimumSize(const Math::Vector2& minimumSize);
    Math::Vector2 getMinimumSize() const;

    void setMaximumSize(const Math::Vector2& maximumSize);
    Math::Vector2 getMaximumSize() const;

    void setPreferredSize(const Math::Vector2& preferredSize);
    Math::Vector2 getPreferredSizeSize() const;

    void setFlexible(bool isFlexible);
    bool isFlexible() const;

    void setForegroundColor(const Graphics::Color& foregroundColor);
    Graphics::Color getForegroundColor() const;

    void setBackgroundColor(const Graphics::Color& backgroundColor);
    Graphics::Color getBackgroundColor() const;

    void setFont(const std::shared_ptr<Graphics::FontMap>& fontMap);
    std::shared_ptr<Graphics::FontMap> getFont() const;

    void setFontSize(int32_t fontSize);
    int32_t getFontSize() const;

private:
    std::weak_ptr<Container> m_parent;

    Math::Vector2 m_size;
    Math::Vector2 m_position;

    Math::Vector2 m_minimumSize;
    Math::Vector2 m_maximumSize;
    Math::Vector2 m_preferredSize;
    bool m_isFlexible;
    Graphics::Color m_foregroundColor;
    Graphics::Color m_backgroundColor;
    std::shared_ptr<Graphics::FontMap> m_fontMap;
    int32_t m_fontSize;
};
}