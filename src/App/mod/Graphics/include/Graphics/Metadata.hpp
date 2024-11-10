// Auto generated. DO NOT EDIT.
#pragma once
#include <Graphics/Reflect.hpp>

namespace Lib::Graphics::Metadata {
    class Uniform {
    public:
        size_t size;
        bool isShaderResource;
    };

    class Program {
    public:
        Reflect::InputLayout inputLayout;
        std::vector<Uniform> vsUniforms;
        std::vector<Uniform> psUniforms;
    };

    enum ProgramTable : int32_t {
        Color2D,
        Text2D,
        Texture2D,
        Count
    };

    const std::vector<Program> k_programs = std::vector<Program> {
        Program {
            // inputLayout
            Reflect::InputLayout::Vertex2D,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), false },
                Uniform { sizeof(Reflect::UVector4), false },
            },
            // psUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexTexCoord2D,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), false },
                Uniform { sizeof(Reflect::UVector4), false },
            },
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, true },
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexTexCoord2D,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), false },
                Uniform { sizeof(Reflect::UVector4), false },
            },
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, true },
            },
        },
    };

    template<int32_t N>
    class Signature {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Color2D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Text2D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Texture2D> {
    public:
        static inline void set() { }
    };

}
