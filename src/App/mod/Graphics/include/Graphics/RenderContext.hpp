#pragma once
#include <Graphics/Metadata.hpp>
#include <d3d12.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Shader;
class UniformBuffer;
class Buffer;
class RenderContext {
public:
    static std::shared_ptr<RenderContext> get(Metadata::ProgramTable entry);

#if SOLID_ENABLE_INTERNAL
    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength,
        const std::vector<std::shared_ptr<Buffer>>& instanceBuffers,
        int32_t instanceCount);

    static void initialize();
    static void destroy();
#endif
private:
    RenderContext();

    static std::vector<std::shared_ptr<RenderContext>> s_table;

    Metadata::ProgramTable m_entry;
    std::shared_ptr<Shader> m_vShader;
    std::shared_ptr<Shader> m_gShader;
    std::shared_ptr<Shader> m_pShader;
    std::shared_ptr<Shader> m_cShader;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_computePipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_computeRootSignature;

    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength,
        const std::shared_ptr<Buffer>* instanceBuffers,
        int32_t instanceBufferCount,
        int32_t instanceCount);
};
}
