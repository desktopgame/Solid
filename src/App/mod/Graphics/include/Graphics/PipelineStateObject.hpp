#pragma once
#include <Graphics/Constant.hpp>
#include <Graphics/PrimitiveType.hpp>
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
        Constant::Layout constantLayout,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool isUsingNormal,
        bool isUsingTexCoord);
    ~PipelineStateObject();

#if SOLID_ENABLE_INTERNAL
    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<Constant> constant,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);
#endif

private:
    PipelineStateObject();

    std::shared_ptr<Shader> m_shader;
    Constant::Layout m_constantLayout;
    PrimitiveType m_primitiveType;
    int32_t m_vertexComponent;
    bool m_isUsingNormal;
    bool m_isUsingTexCoord;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
#endif
};
}