#pragma once
#include <Graphics/RenderInterface.hpp>
#include <Graphics/Surface.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::Graphics::Internal {
class Constant;
}

namespace Lib::Graphics {
class Shader;
class Buffer;
class Texture;
class RenderParameter {
public:
    static std::shared_ptr<RenderParameter> create(RenderInterface interfaze);
    ~RenderParameter();

    void update();

    void setTransform(const Math::Matrix& transform);
    Math::Matrix getTransform() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> getTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;

    RenderInterface getInterface() const;

private:
    RenderParameter();
    std::shared_ptr<Internal::Constant> getConstant() const;
    friend void Surface::render(const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<RenderParameter>& renderParameter,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool isUsingTexCoord,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

    std::shared_ptr<Internal::Constant> m_constant;
};
}