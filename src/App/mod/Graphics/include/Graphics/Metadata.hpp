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
        std::vector<Reflect::InstanceBufferType> instanceBufferLayout;
        Reflect::PrimitiveType primitiveType;
        bool isWireframe;
        const char* vsCode;
        std::vector<Uniform> vsUniforms;
        const char* psCode;
        std::vector<Uniform> psUniforms;
    };

    enum ProgramTable : int32_t {
        Color2D,
        Text2D,
        Texture2D,
        MeshColor3D,
        MeshTexture3D,
        MeshWireframe3D,
        MeshInstanceColor3D,
        Count
    };

    const std::vector<Program> k_programs = std::vector<Program> {
        Program {
            // inputLayout
            Reflect::InputLayout::Vertex2D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
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
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float2 texCoord : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
            ""
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.texCoord = texCoord;"
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
            "    float2 texCoord : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            ""
            "Texture2D<float4> tex : register(t0);"
            "SamplerState smp : register(s0);"
            ""
            "float4 psMain(Output input) : SV_TARGET {"
            "    float4 col = float4(tex.Sample(smp, input.texCoord));"
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
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float2 texCoord : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
            ""
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.texCoord = texCoord;"
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
            "    float2 texCoord : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            ""
            "Texture2D<float4> tex : register(t0);"
            "SamplerState smp : register(s0);"
            ""
            "float4 psMain(Output input) : SV_TARGET {"
            "    float4 col = float4(tex.Sample(smp, input.texCoord)) * input.color;"
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
            Reflect::InputLayout::VertexNormal3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float3 wpos : POSITION;"
            "    float3 normal : NORMAL;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
            ""
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));"
            "    output.normal = normal;"
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
            "    float3 wpos : POSITION;"
            "    float3 normal : NORMAL;"
            "    float4 color : COLOR;"
            "};"
            "struct PSOutput"
            "{"
            "    float4 outPosition : SV_Target0;"
            "    float4 outNormal : SV_Target1;"
            "    float4 outColor : SV_Target2;"
            "};"
            ""
            "PSOutput psMain(Output input) : SV_TARGET {"
            "    PSOutput output;"
            "    output.outPosition = float4(input.wpos, 1);"
            "    output.outNormal = float4(input.normal, 1);"
            "    output.outColor = input.color;"
            "    return output;"
            "}"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexNormalTexCoord3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float3 wpos : POSITION;"
            "    float3 normal : NORMAL;"
            "    float2 texCoord : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
            ""
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));"
            "    output.normal = normal;"
            "    output.texCoord = texCoord;"
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
            "    float3 wpos : POSITION;"
            "    float3 normal : NORMAL;"
            "    float2 texCoord : TEXCOORD;"
            "    float4 color : COLOR;"
            "};"
            "struct PSOutput"
            "{"
            "    float4 outPosition : SV_Target0;"
            "    float4 outNormal : SV_Target1;"
            "    float4 outColor : SV_Target2;"
            "};"
            ""
            "Texture2D<float4> tex : register(t0);"
            "SamplerState smp : register(s0);"
            ""
            "PSOutput psMain(Output input) : SV_TARGET {"
            "    PSOutput output;"
            "    output.outPosition = float4(input.wpos, 1);"
            "    output.outNormal = float4(input.normal, 1);"
            "    output.outColor = float4(tex.Sample(smp, input.texCoord)) * input.color;"
            "    return output;"
            "}"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, true },
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::Vertex3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            true,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
            ""
            "Output vsMain(float3 pos : POSITION) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));"
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
            "struct PSOutput"
            "{"
            "    float4 outPosition : SV_Target0;"
            "    float4 outNormal : SV_Target1;"
            "    float4 outColor : SV_Target2;"
            "};"
            ""
            "PSOutput psMain(Output input) : SV_TARGET {"
            "    PSOutput output;"
            "    output.outColor = input.color;"
            "    return output;"
            "}"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexNormal3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
                Reflect::InstanceBufferType::Vector3,
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // vs
            "struct Output {"
            "    float4 svpos : SV_POSITION;"
            "    float3 wpos : POSITION;"
            "    float3 normal : NORMAL;"
            "    float4 color : COLOR;"
            "};"
            "cbuffer cbuff0 : register(b0) {"
            "    matrix modelMatrix;"
            "    matrix viewMatrix;"
            "    matrix projectionMatrix;"
            "}"
            "cbuffer cbuff1 : register(b1) { float4 color; }"
            ""
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float3 offset: INSTANCE0) {"
            "    Output output;"
            "    output.svpos = mul(modelMatrix, float4(pos + offset, 1));"
            "    output.svpos = mul(viewMatrix, output.svpos);"
            "    output.svpos = mul(projectionMatrix, output.svpos);"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));"
            "    output.normal = normal;"
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
            "    float3 wpos : POSITION;"
            "    float3 normal : NORMAL;"
            "    float4 color : COLOR;"
            "};"
            "struct PSOutput"
            "{"
            "    float4 outPosition : SV_Target0;"
            "    float4 outNormal : SV_Target1;"
            "    float4 outColor : SV_Target2;"
            "};"
            ""
            "PSOutput psMain(Output input) : SV_TARGET {"
            "    PSOutput output;"
            "    output.outPosition = float4(input.wpos, 1);"
            "    output.outNormal = float4(input.normal, 1);"
            "    output.outColor = input.color;"
            "    return output;"
            "}"
            ,
            // psUniforms
            std::vector<Uniform> {
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

    template<>
    class Signature<ProgramTable::MeshColor3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::MeshTexture3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::MeshWireframe3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::MeshInstanceColor3D> {
    public:
        static inline void set() { }
    };

}
