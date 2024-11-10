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
        const char* vsCode;
        std::vector<Uniform> vsUniforms;
        const char* psCode;
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
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) { matrix modelMatrix; }"
            "cbuffer cbuff1 : register(b1) { matrix viewMatrix; }"
            "cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }"
            "cbuffer cbuff3 : register(b3) { float4 color; }"
            ""
            "Output vsMain(float2 pos : POSITION) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.color = color;"
            "    return output;"
            "}"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), false },
                Uniform { sizeof(Reflect::UVector4), false },
            },
            // ps
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float4 color : COLOR;"
            "};"
            ""
            "float4 psMain(Output input) : SV_TARGET {"
            "    return input.color;"
            "}"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexTexCoord2D,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float2 uv : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) { matrix modelMatrix; }"
            "cbuffer cbuff1 : register(b1) { matrix viewMatrix; }"
            "cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }"
            "cbuffer cbuff3 : register(b3) { float4 color; }"
            ""
            "Output vsMain(float2 pos : POSITION, float2 uv : TEXCOORD) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.uv = uv;"
            "    output.color = color;"
            "    return output;"
            "}"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), false },
                Uniform { sizeof(Reflect::UVector4), false },
            },
            // ps
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float2 uv : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            ""
            "Texture2D<float4> tex : register(t0);"
            "SamplerState smp : register(s0);"
            ""
            "float4 psMain(Output input) : SV_TARGET {"
            "    float4 col = float4(tex.Sample(smp, input.uv));"
            "    col.w = col.x * input.color.w;"
            "    col.x = input.color.x;"
            "    col.y = input.color.y;"
            "    col.z = input.color.z;"
            "    return col;"
            "}"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, true },
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexTexCoord2D,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float2 uv : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) { matrix modelMatrix; }"
            "cbuffer cbuff1 : register(b1) { matrix viewMatrix; }"
            "cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }"
            "cbuffer cbuff3 : register(b3) { float4 color; }"
            ""
            "Output vsMain(float2 pos : POSITION, float2 uv : TEXCOORD) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.uv = uv;"
            "    output.color = color;"
            "    return output;"
            "}"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), false },
                Uniform { sizeof(Reflect::UVector4), false },
            },
            // ps
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float2 uv : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            ""
            "Texture2D<float4> tex : register(t0);"
            "SamplerState smp : register(s0);"
            ""
            "float4 psMain(Output input) : SV_TARGET {"
            "    float4 col = float4(tex.Sample(smp, input.uv)) * input.color;"
            "    return col;"
            "}"
            ,
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
