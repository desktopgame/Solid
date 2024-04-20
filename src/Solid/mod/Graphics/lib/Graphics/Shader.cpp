#include <Graphics/Shader.hpp>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <stdexcept>
#include <wrl/client.h>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// Program
Shader::Program::Program(const void* pointer, size_t size)
    : m_pointer(pointer)
    , m_size(size)
{
}
const void* Shader::Program::getPointer() const { return m_pointer; }
size_t Shader::Program::getSize() const { return m_size; }
// Impl
class Shader::Impl {
public:
    explicit Impl() = default;
    ComPtr<ID3DBlob> m_vertexBlob;
    ComPtr<ID3DBlob> m_pixelBlob;
};
// public
Shader::~Shader()
{
}

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
            &shader->m_impl->m_vertexBlob,
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
            &shader->m_impl->m_pixelBlob,
            &errorMessage))) {
        std::string errorString;
        errorString.resize(errorMessage->GetBufferSize());
        std::copy_n((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize(), errorString.begin());
        throw std::logic_error(errorString);
    }
    return shader;
}

Shader::Program Shader::getVertexProgram() const
{
    LPVOID ptr = m_impl->m_vertexBlob->GetBufferPointer();
    SIZE_T size = m_impl->m_vertexBlob->GetBufferSize();
    return Program(static_cast<void*>(ptr), static_cast<size_t>(size));
}
Shader::Program Shader::getPixelProgram() const
{
    LPVOID ptr = m_impl->m_pixelBlob->GetBufferPointer();
    SIZE_T size = m_impl->m_pixelBlob->GetBufferSize();
    return Program(static_cast<void*>(ptr), static_cast<size_t>(size));
}
// private
Shader::Shader()
    : m_impl(std::make_shared<Impl>())
{
}
}