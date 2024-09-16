#pragma once
#include <Graphics/TileBuffer.hpp>
#include <Math/Vector.hpp>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Shader;
class Buffer;
class Constant;
class TileBatch {
public:
    static std::shared_ptr<TileBatch> create(const std::shared_ptr<ITileBuffer> tileBuffer);
    ~TileBatch();

    int32_t rent();
    void release(int32_t index);

#if SOLID_ENABLE_INTERNAL
    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
#endif

private:
    TileBatch();

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
    std::shared_ptr<Buffer> m_constantBuffer;
    std::shared_ptr<Buffer> m_commandBuffer;
    std::shared_ptr<ITileBuffer> m_tileBuffer;
    std::vector<bool> m_commandVisibleTable;
    std::vector<int32_t> m_commandIndexTable;
    int32_t m_indexLength;
    bool m_shouldCompact;
    bool m_shouldCommandCopy;

#if SOLID_ENABLE_INTERNAL
    struct IndirectCommand {
    public:
        explicit IndirectCommand() = default;
        D3D12_GPU_VIRTUAL_ADDRESS cbv;
        D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
    };

    std::vector<IndirectCommand> m_commands;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12CommandSignature> m_commandSignature;
#endif
};
}