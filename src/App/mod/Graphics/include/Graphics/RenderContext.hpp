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
class RenderContext {
public:
    static std::shared_ptr<RenderContext> get(Metadata::ProgramTable entry);

#if SOLID_ENABLE_INTERNAL
    static void initialize();
    static void destroy();
#endif
private:
    RenderContext();

    static std::vector<std::shared_ptr<RenderContext>> s_table;

    Metadata::Program m_program;
    std::shared_ptr<Shader> m_shader;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
#endif
};
}
