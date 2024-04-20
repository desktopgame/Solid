#pragma once
#include <Graphics/VertexData2D.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class RenderParameter;
class Buffer;
class RenderContext {
public:
    static std::shared_ptr<RenderContext> create();

    ~RenderContext();

    void updateVertex(const Math::Vector2* data, int32_t len);
    void updateVertex(const VertexData2D* data, int32_t len);
    void updateVertex(const Math::Vector3* data, int32_t len);
    void updateIndex(uint32_t* data, int32_t len);

    int32_t getVertexComponent() const;

    uint64_t getVertexVirtualAddress() const;
    int32_t getVertexLength() const;
    uint64_t getIndexVirtualAddress() const;
    int32_t getIndexLength() const;

    bool isUsingTexCoord() const;

private:
    RenderContext();

    std::shared_ptr<RenderParameter> m_parameter;
    int32_t m_vertexComponent;
    int32_t m_vertexLength;
    int32_t m_indexLength;
    bool m_isUsingTexCoord;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
};
}