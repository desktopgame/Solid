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
class PipelineStateObject {
public:
    static std::shared_ptr<PipelineStateObject> create(
        const std::shared_ptr<Shader>& shader,
        RenderInterface renderInterface,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool usingTexCoord);
    ~PipelineStateObject();

    std::shared_ptr<Shader> getShader() const;
    RenderInterface getRenderInterface() const;
    PrimitiveType getPrimitiveType() const;
    int32_t getVertexComponent() const;
    bool isUsingTexCoord() const;

#if SOLID_ENABLE_INTERNAL
    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<RenderParameter> renderParameter,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);
#endif

private:
    PipelineStateObject();

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