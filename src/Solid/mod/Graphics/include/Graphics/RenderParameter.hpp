#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::Graphics {
class RenderParameter {
public:
    RenderParameter();

    void setTransform(const Math::Matrix& transform);
    Math::Matrix getTransform() const;

    bool useTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;
    bool useColor() const;

private:
    Math::Matrix m_transform;

    Math::Vector4 m_color;
    bool m_useColor;
};
}