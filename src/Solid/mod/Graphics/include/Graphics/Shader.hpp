#pragma once
#include <memory>
#include <string>

namespace Lib::Graphics {
class Shader {
public:
    ~Shader();

    class Program {
    public:
        explicit Program(const void* pointer, size_t size);
        const void* getPointer() const;
        size_t getSize() const;

    private:
        const void* m_pointer;
        size_t m_size;
    };

    static std::shared_ptr<Shader> compile(
        const std::string& vertexShader,
        const std::string& vertexEntrypoint,
        const std::string& pixelShader,
        const std::string& pixelEntrypoint);

    Program getVertexProgram() const;
    Program getPixelProgram() const;

private:
    Shader();

    class Impl;
    std::shared_ptr<Impl> m_impl;
};
}