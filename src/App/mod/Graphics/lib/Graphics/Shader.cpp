#include <Graphics/Shader.hpp>
#include <iostream>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<Shader> Shader::compile(
    const std::string& stage,
    const std::string& entryPoint,
    const std::string& shaderCode,
    const std::string& shaderName)
{
    auto shader = std::shared_ptr<Shader>(new Shader());
    ComPtr<ID3DBlob> errorMessage = nullptr;
    UINT flags = 0;
#if _DEBUG
    flags = D3DCOMPILE_DEBUG;
#endif
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;

    if (FAILED(D3DCompile(
            shaderCode.c_str(),
            shaderCode.size(),
            shaderName.c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.c_str(),
            stage.c_str(),
            flags,
            0,
            &shader->m_blob,
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
void Shader::getD3D12_SHADER_BYTECODE(D3D12_SHADER_BYTECODE& output) const
{
    output.pShaderBytecode = m_blob->GetBufferPointer();
    output.BytecodeLength = m_blob->GetBufferSize();
}
// private
Shader::Shader()
    : m_blob()
{
}
}