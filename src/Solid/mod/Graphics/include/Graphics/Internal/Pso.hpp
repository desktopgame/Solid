#pragma once
#include <Graphics/PrimitiveType.hpp>
#include <Graphics/RenderParameter.hpp>
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class Shader;
}
class ID3D12GraphicsCommandList;
namespace Lib::Graphics::Internal {
class Pso {
public:
    static std::shared_ptr<Pso> create(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<RenderParameter>& renderParameter,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool usingTexCoord);

    ~Pso();
    void update();
    void command(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

private:
    Pso();

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<RenderParameter> m_renderParameter;
    PrimitiveType m_primitiveType;
    int32_t m_vertexComponent;
    bool m_isUsingTexCoord;
    bool m_isDirty;

    class Impl;
    std::shared_ptr<Impl> m_impl;
};
}