#pragma once
#include <memory>
#include <string>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Shader {
public:
    static std::shared_ptr<Shader> compile(
        const std::string& vertexShader,
        const std::string& vertexEntrypoint,
        const std::string& pixelShader,
        const std::string& pixelEntrypoint);
    ~Shader();

#if SOLID_ENABLE_INTERNAL
    void getD3D12_SHADER_BYTECODE(
        D3D12_SHADER_BYTECODE& outVS,
        D3D12_SHADER_BYTECODE& outPS) const;
#endif

private:
    Shader();

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3DBlob> m_vertexBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> m_pixelBlob;
#endif
};
}