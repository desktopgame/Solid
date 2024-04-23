#pragma once
#include <Graphics/PrimitiveType.hpp>
#include <Graphics/VertexData2D.hpp>
#include <Graphics/VertexData3D.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class RenderParameter;
class Buffer;
class Surface;
class Shader;
class RenderContext {
public:
    static std::shared_ptr<RenderContext> create(PrimitiveType primitiveType);

    ~RenderContext();

    void updateVertex(const Math::Vector2* data, int32_t len);
    void updateVertex(const VertexData2D* data, int32_t len);
    void updateVertex(const Math::Vector3* data, int32_t len);
    void updateVertex(const VertexData3D* data, int32_t len);
    void updateIndex(const uint32_t* data, int32_t len);

    PrimitiveType getPrimitiveType() const;
    int32_t getVertexComponent() const;
    int32_t getIndexLength() const;
    bool isUsingTexCoord() const;
    const std::shared_ptr<Buffer>& getVertexBuffer() const;
    const std::shared_ptr<Buffer>& getIndexBuffer() const;

private:
    RenderContext(PrimitiveType primitiveType);

    PrimitiveType m_primitiveType;
    int32_t m_vertexComponent;
    int32_t m_vertexLength;
    int32_t m_indexLength;
    bool m_isUsingTexCoord;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
};
}