#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::Graphics {
class Texture;
class RenderParameter {
public:
    RenderParameter();

    void setTransform(const Math::Matrix& transform);
    Math::Matrix getTransform() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> getTexture() const;
    bool useTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;
    bool useColor() const;

private:
    Math::Matrix m_transform;

    std::shared_ptr<Texture> m_texture;
    bool m_useTexture;

    Math::Vector4 m_color;
    bool m_useColor;
};
}