#pragma once
#include <Graphics/Metadata.hpp>
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>

class ID3D12GraphicsCommandList;
#endif

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
        const std::vector<std::shared_ptr<Buffer>>& instanceBuffers,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

    static void initialize();
    static void destroy();
#endif
private:
    RenderContext();

    static std::vector<std::shared_ptr<RenderContext>> s_table;

    Metadata::ProgramTable m_entry;
    std::shared_ptr<Shader> m_shader;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;

    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<Buffer>* instanceBuffers,
        int32_t instanceBufferCount,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);
#endif
};
}
