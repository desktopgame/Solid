#pragma once
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <memory>
#include <string>
#include <wrl/client.h>

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

    Microsoft::WRL::ComPtr<ID3DBlob> m_vertexBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> m_pixelBlob;
};
}