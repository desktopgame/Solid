#include <Graphics/Shader.hpp>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<Shader> Shader::compile(
    const std::string& vertexShader,
    const std::string& vertexEntrypoint,
    const std::string& pixelShader,
    const std::string& pixelEntrypoint)
{
    auto shader = std::shared_ptr<Shader>(new Shader());
    ComPtr<ID3DBlob> errorMessage = nullptr;
    UINT flags = 0;
#if _DEBUG
    flags = D3DCOMPILE_DEBUG;
#endif
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
    // compile VertexShader
    if (FAILED(D3DCompile(
            vertexShader.c_str(),
            vertexShader.size(),
            "VertexShader",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            vertexEntrypoint.c_str(),
            "vs_5_0",
            flags,
            0,
            &shader->m_vertexBlob,
            &errorMessage))) {
        std::string errorString;
        errorString.resize(errorMessage->GetBufferSize());
        std::copy_n((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize(), errorString.begin());
        throw std::logic_error(errorString);
    }
    // compile PixelShader
    if (FAILED(D3DCompile(
            pixelShader.c_str(),
            pixelShader.size(),
            "FragmentShader",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            pixelEntrypoint.c_str(),
            "ps_5_0",
            flags,
            0,
            &shader->m_pixelBlob,
            &errorMessage))) {
        std::string errorString;
        errorString.resize(errorMessage->GetBufferSize());
        std::copy_n((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize(), errorString.begin());
        throw std::logic_error(errorString);
    }
    return shader;
}

Shader::~Shader()
{
}
// internal
void Shader::getD3D12_SHADER_BYTECODE(
    D3D12_SHADER_BYTECODE& outVS,
    D3D12_SHADER_BYTECODE& outPS) const
{
    outVS.pShaderBytecode = m_vertexBlob->GetBufferPointer();
    outVS.BytecodeLength = m_vertexBlob->GetBufferSize();
    outPS.pShaderBytecode = m_pixelBlob->GetBufferPointer();
    outPS.BytecodeLength = m_pixelBlob->GetBufferSize();
}
// private
Shader::Shader()
    : m_vertexBlob()
    , m_pixelBlob()
{
}
}