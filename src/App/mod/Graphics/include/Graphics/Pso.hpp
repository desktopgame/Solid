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
// class Constant;
class Pso {
public:
    static std::shared_ptr<Pso> create(
        const std::shared_ptr<Shader>& shader,
        RenderInterface renderInterface,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool usingTexCoord);

    ~Pso();
    void command(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<RenderParameter> renderParameter);

private:
    Pso();

    std::shared_ptr<Shader> m_shader;
    RenderInterface m_renderInterface;
    PrimitiveType m_primitiveType;
    int32_t m_vertexComponent;
    bool m_isUsingTexCoord;

    class Impl;
    std::shared_ptr<Impl> m_impl;
};
}