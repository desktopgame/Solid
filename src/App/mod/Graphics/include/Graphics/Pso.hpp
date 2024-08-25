#pragma once
#include <Graphics/PrimitiveType.hpp>
#include <Graphics/RenderParameter.hpp>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>

class ID3D12GraphicsCommandList;
#endif

namespace Lib::Graphics {
class Shader;
}
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

#if SOLID_ENABLE_INTERNAL
    void command(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<RenderParameter> renderParameter);
#endif

private:
    Pso();

    std::shared_ptr<Shader> m_shader;
    RenderInterface m_renderInterface;
    PrimitiveType m_primitiveType;
    int32_t m_vertexComponent;
    bool m_isUsingTexCoord;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
#endif
};
}