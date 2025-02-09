#pragma once
#include <Graphics/Renderer.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::UI {
class Container;
class Component {
public:
    explicit Component();
    virtual ~Component();

    virtual void update() = 0;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) = 0;

    void setParent(const std::shared_ptr<Container>& parent);
    std::shared_ptr<Container> getParent() const;

    void setSize(const Math::Vector2& size);
    Math::Vector2 getSize() const;

    void setPosition(const Math::Vector2& position);
    Math::Vector2 getPosition() const;

    void setMinimumSize(const Math::Vector2& minimumSize);
    Math::Vector2 getMinimumSize() const;

    void setMaximumSize(const Math::Vector2& maximumSize);
    Math::Vector2 getMaximumSize() const;

private:
    std::weak_ptr<Container> m_parent;

    Math::Vector2 m_size;
    Math::Vector2 m_position;

    Math::Vector2 m_minimumSize;
    Math::Vector2 m_maximumSize;
};
}