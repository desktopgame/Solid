#pragma once
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Shader;
class Buffer;
class TileBatch {
public:
    static std::shared_ptr<TileBatch> create();
    ~TileBatch();

private:
    TileBatch();

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
#endif
};
}