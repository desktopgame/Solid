#pragma once
#include <Graphics/IBuffer.hpp>
#include <Graphics/Metadata.hpp>
#include <d3d12.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Shader;
class UniformBuffer;
/**
 * 描画に関する頂点情報以外のコンテキストを保持するクラスです。
 * 具体的には、シェーダーとパイプラインステートを持ちます。
 * 外部からこれに頂点情報とユニフォームさえ渡せばすぐに描画を実行できます。
 */
class RenderContext {
public:
    /**
     * 指定のプログラムのレンダーコンテキストを返します。
     * @param entry
     * @return
     */
    static std::shared_ptr<RenderContext> get(Metadata::ProgramTable entry);

    /**
     * この呼び出しに続く描画が頂点情報を共有できることをマークします。
     * この場合には、少しだけ命令を省略します。
     */
    void beginBatch();

    /**
     * 頂点情報の共有を終了します。
     */
    void endBatch();

#if SOLID_ENABLE_INTERNAL
    void compute(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        int32_t threadGroupCountX,
        int32_t threadGroupCountY,
        int32_t threadGroupCountZ);

    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<IBuffer>& vertexBuffer,
        const std::shared_ptr<IBuffer>& indexBuffer,
        int32_t indexLength);

    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<IBuffer>& vertexBuffer,
        const std::shared_ptr<IBuffer>& indexBuffer,
        int32_t indexLength,
        const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
        int32_t instanceCount);

    static void initialize();
    static void destroy();
#endif
private:
    RenderContext();
    void init(Metadata::ProgramTable entry);

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

    bool m_batchMode;
    bool m_batchInit;

    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<IBuffer>& vertexBuffer,
        const std::shared_ptr<IBuffer>& indexBuffer,
        int32_t indexLength,
        const std::shared_ptr<IBuffer>* instanceBuffers,
        int32_t instanceBufferCount,
        int32_t instanceCount);
};
}
