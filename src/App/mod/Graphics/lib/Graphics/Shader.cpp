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
Microsoft::WRL::ComPtr<ID3DBlob> Shader::getVertexShaderBlob() const { return m_vertexBlob; }

Microsoft::WRL::ComPtr<ID3DBlob> Shader::getPixelShaderBlob() const { return m_pixelBlob; }
// private
Shader::Shader()
    : m_vertexBlob()
    , m_pixelBlob()
{
}
}