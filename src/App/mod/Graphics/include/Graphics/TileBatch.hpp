#pragma once
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
    static std::shared_ptr<TileBatch> create();
    ~TileBatch();

#if SOLID_ENABLE_INTERNAL
    struct ConstantData {
    public:
        explicit ConstantData() = default;
        std::array<Math::Vector4, 16> tiles; // 4byte * 4 * 16 = 256byte
        Math::Matrix matrix;
    };

    struct IndirectCommand {
    public:
        explicit IndirectCommand() = default;
        D3D12_GPU_VIRTUAL_ADDRESS cbv;
        D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
    };

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
    int32_t m_indexLength;

#if SOLID_ENABLE_INTERNAL
    std::vector<TileBatch::ConstantData> m_constantBufferData;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12CommandSignature> m_commandSignature;
#endif
};
}