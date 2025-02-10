// Auto generated. DO NOT EDIT.
#pragma once
#include <Graphics/Reflect.hpp>

namespace Lib::Graphics::Metadata {
    class Uniform {
    public:
        enum class Type : uint32_t {
            CBV = 0,
            SRV,
            UAV,
        };
        size_t size;
        Type type;
    };

    class Stencil {
    public:
        bool isRead;
        bool isWrite;
        Reflect::StencilOp frontFaceFailOp;
        Reflect::StencilOp frontFaceDepthFailOp;
        Reflect::StencilOp frontFacePassOp;
        Reflect::StencilFunc frontFaceFunc;
        Reflect::StencilOp backFaceFailOp;
        Reflect::StencilOp backFaceDepthFailOp;
        Reflect::StencilOp backFacePassOp;
        Reflect::StencilFunc backFaceFunc;
    };

    class Program {
    public:
        Reflect::InputLayout inputLayout;
        std::vector<Reflect::InstanceBufferType> instanceBufferLayout;
        Reflect::PrimitiveType primitiveType;
        bool isWireframe;
        Stencil stencil;
        const char* vsCode;
        std::vector<Uniform> vsUniforms;
        const char* gsCode;
        std::vector<Uniform> gsUniforms;
        const char* psCode;
        std::vector<Uniform> psUniforms;
        const char* csCode;
        std::vector<Uniform> csUniforms;
    };

    enum ProgramTable : int32_t {
        Color2D,
        Color2D_StencilRead,
        Color2D_StencilWrite,
        Text2D,
        Text2D_StencilRead,
        Text2D_StencilWrite,
        Texture2D,
        Texture2D_StencilRead,
        Texture2D_StencilWrite,
        MeshColor3D,
        MeshTexture3D,
        MeshWireframe3D,
        MeshLine3D,
        MeshInstanceColor3D,
        MeshInstanceTexture3D,
        TileInstance3D,
        ParticleInstance3D,
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    return input.color;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
            },
        },
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
            // stencil
            Stencil { true, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Equal, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    return input.color;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
            },
        },
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
            // stencil
            Stencil { false, true, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Replace, Reflect::StencilFunc::Always, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    return input.color;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    float4 col = float4(tex.Sample(smp, input.texCoord));\n"
            "    col.w = col.x * input.color.w;\n"
            "    col.x = input.color.x;\n"
            "    col.y = input.color.y;\n"
            "    col.z = input.color.z;\n"
            "    if (col.w <= 0) {\n"
            "        discard;\n"
            "    }\n"
            "    return col;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { true, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Equal, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    float4 col = float4(tex.Sample(smp, input.texCoord));\n"
            "    col.w = col.x * input.color.w;\n"
            "    col.x = input.color.x;\n"
            "    col.y = input.color.y;\n"
            "    col.z = input.color.z;\n"
            "    if (col.w <= 0) {\n"
            "        discard;\n"
            "    }\n"
            "    return col;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { false, true, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Replace, Reflect::StencilFunc::Always, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    float4 col = float4(tex.Sample(smp, input.texCoord));\n"
            "    col.w = col.x * input.color.w;\n"
            "    col.x = input.color.x;\n"
            "    col.y = input.color.y;\n"
            "    col.z = input.color.z;\n"
            "    if (col.w <= 0) {\n"
            "        discard;\n"
            "    }\n"
            "    return col;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    float4 col = float4(tex.Sample(smp, input.texCoord)) * input.color;\n"
            "    return col;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { true, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Equal, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    float4 col = float4(tex.Sample(smp, input.texCoord)) * input.color;\n"
            "    return col;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { false, true, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Replace, Reflect::StencilFunc::Always, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 0, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "float4 psMain(Output input) : SV_TARGET {\n"
            "    float4 col = float4(tex.Sample(smp, input.texCoord)) * input.color;\n"
            "    return col;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.normal = normal;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    PSOutput output;\n"
            "    output.outPosition = float4(input.wpos, 1);\n"
            "    output.outNormal = float4(input.normal, 1);\n"
            "    output.outColor = input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.normal = normal;\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    PSOutput output;\n"
            "    output.outPosition = float4(input.wpos, 1);\n"
            "    output.outNormal = float4(input.normal, 1);\n"
            "    output.outColor = float4(tex.Sample(smp, input.texCoord)) * input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 wpos : POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.wpos = output.svpos;\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 wpos : POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    PSOutput output;\n"
            "    output.outPosition = input.wpos;\n"
            "    output.outColor = input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
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
            false,
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 pos: POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.pos = output.svpos;\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            "struct GSInput {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 pos : POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "struct GSOutput {\n"
            "    float4 position : SV_POSITION;\n"
            "    float4 wpos : POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "\n"
            "cbuffer cbuff1 : register(b1) {\n"
            "    float lineWidth;\n"
            "}\n"
            "\n"
            "cbuffer cbuff2 : register(b2) {\n"
            "    float4 cameraPosition;\n"
            "}\n"
            "\n"
            "[maxvertexcount(18)]\n"
            "void gsMain(triangle GSInput input[3], inout TriangleStream<GSOutput> triStream) {\n"
            "    matrix vpMat = mul(projectionMatrix, viewMatrix);\n"
            "    for (int i = 0; i < 3; ++i) {\n"
            "        int nextIndex = (i + 1) % 3;\n"
            "\n"
            "        float3 worldP0 = input[i].pos;\n"
            "        float3 worldP1 = input[nextIndex].pos;\n"
            "\n"
            "        float3 camToEdge = normalize(worldP0 - cameraPosition.xyz);\n"
            "        float3 edgeDir = normalize(worldP1 - worldP0);\n"
            "        float3 edgeNormal = normalize(cross(edgeDir, camToEdge));\n"
            "\n"
            "        float3 offset = edgeNormal * (lineWidth * 0.5);\n"
            "\n"
            "        GSOutput v0, v1, v2, v3;\n"
            "\n"
            "        v0.position = mul(vpMat, float4(worldP0 + offset, 1.0));\n"
            "        v0.wpos = float4(worldP0 + offset, 1.0);\n"
            "        v0.color = input[i].color;\n"
            "\n"
            "        v1.position = mul(vpMat, float4(worldP1 + offset, 1.0));\n"
            "        v1.wpos = float4(worldP1 + offset, 1.0);\n"
            "        v1.color = input[nextIndex].color;\n"
            "\n"
            "        v2.position = mul(vpMat, float4(worldP0 - offset, 1.0));\n"
            "        v2.wpos = float4(worldP0 - offset, 1.0);\n"
            "        v2.color = input[i].color;\n"
            "\n"
            "        v3.position = mul(vpMat, float4(worldP1 - offset, 1.0));\n"
            "        v3.wpos = float4(worldP1 - offset, 1.0);\n"
            "        v3.color = input[nextIndex].color;\n"
            "\n"
            "        triStream.Append(v0);\n"
            "        triStream.Append(v1);\n"
            "        triStream.Append(v2);\n"
            "\n"
            "        triStream.Append(v2);\n"
            "        triStream.Append(v1);\n"
            "        triStream.Append(v3);\n"
            "    }\n"
            "}\n"
            ,
            // gsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UFloat), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 wpos : POSITION;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    PSOutput output;\n"
            "    output.outPosition = input.wpos;\n"
            "    output.outColor = input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
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
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float3 offset: INSTANCE0) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos + offset, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.normal = normal;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    PSOutput output;\n"
            "    output.outPosition = float4(input.wpos, 1);\n"
            "    output.outNormal = float4(input.normal, 1);\n"
            "    output.outColor = input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexNormalTexCoord3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
                Reflect::InstanceBufferType::Vector3,
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD, float3 offset: INSTANCE0) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos + offset, 1));\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.wpos = mul(modelMatrix, float4(pos, 1));\n"
            "    output.normal = normal;\n"
            "    output.texCoord = texCoord;\n"
            "    output.color = color;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 color : COLOR;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    PSOutput output;\n"
            "    output.outPosition = float4(input.wpos, 1);\n"
            "    output.outNormal = float4(input.normal, 1);\n"
            "    output.outColor = float4(tex.Sample(smp, input.texCoord)) * input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexNormalTexCoord3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
                Reflect::InstanceBufferType::Vector4,
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 mmpos : POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 axis : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "    float3 cameraPosition : POSITION1;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "};\n"
            "cbuffer cbuff1 : register(b1)\n"
            "{\n"
            "    matrix translateMatrixTable[6];\n"
            "    matrix rotationMatrixTable[6];\n"
            "    float3 normalVectorTable[6];\n"
            "};\n"
            "cbuffer cbuff2 : register(b2)\n"
            "{\n"
            "    float4 colorTable[64];\n"
            "};\n"
            "cbuffer cbuff3 : register(b3)\n"
            "{\n"
            "    float3 cameraPosition;\n"
            "};\n"
            "\n"
            "static const float4 axisTable[6] = {\n"
            "    // posY\n"
            "    float4(-1, 0, 0, 90),\n"
            "    // negY\n"
            "    float4(1, 0, 0, 90),\n"
            "    // posX\n"
            "    float4(0, 1, 0, 90),\n"
            "    // negX\n"
            "    float4(0, -1, 0, 90),\n"
            "    // posZ\n"
            "    float4(0, 0, 1, 0),\n"
            "    // negZ\n"
            "    float4(0, 1, 0, 180)\n"
            "};\n"
            "\n"
            "Output vsMain(float3 pos : POSITION, float2 texCoord : TEXCOORD, float4 tileData : INSTANCE0) {\n"
            "    Output output;\n"
            "    int tileInfo = int(tileData.w);\n"
            "    int tileRotationID = tileInfo % 10;\n"
            "    int tileColorID = tileInfo / 10;\n"
            "    matrix tileRotation = rotationMatrixTable[tileRotationID];\n"
            "    matrix tileTranslate = translateMatrixTable[tileRotationID];\n"
            "    matrix tileTransform = mul(tileTranslate, tileRotation);\n"
            "    float3 tileOffset = tileData.xyz;\n"
            "\n"
            "    float4 tmp = float4(mul(tileTransform, float4(pos, 1)) + tileOffset, 1);\n"
            "\n"
            "    // メッシュのつなぎ目（gap, seamsなどと呼ばれる）が描画されないための対策\n"
            "    // 隙間なくタイルを敷き詰めているつもりでも、\n"
            "    // 浮動小数点数演算の誤差によってタイルの間に隙間が生じることがある。\n"
            "    // これを抑えるために、小数点第二位以下を切り捨てる。\n"
            "    tmp = round(tmp * 100.0f) / 100.0f;\n"
            "\n"
            "    tmp = mul(modelMatrix, tmp);\n"
            "    output.mmpos = tmp;\n"
            "\n"
            "    tmp = mul(viewMatrix, tmp);\n"
            "    tmp = mul(projectionMatrix, tmp);\n"
            "\n"
            "    output.svpos = tmp;\n"
            "    output.color = colorTable[tileColorID];\n"
            "    output.texCoord = texCoord;\n"
            "    output.cameraPosition = cameraPosition;\n"
            "\n"
            "    float3 normal = normalVectorTable[tileRotationID];\n"
            "    output.axis = axisTable[tileRotationID];\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UTileTransform), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UTilePallet), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector3), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float4 mmpos : POSITION;\n"
            "    float2 texCoord : TEXCOORD;\n"
            "    float4 axis : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "    float3 cameraPosition : POSITION1;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "Texture2D<float4> tex : register(t0);\n"
            "SamplerState smp : register(s0);\n"
            "\n"
            "Texture2D<float4> borderTex : register(t1);\n"
            "SamplerState borderSmp : register(s1);\n"
            "\n"
            "float4 quatConj(float4 quat)\n"
            "{\n"
            "    return float4(quat.xyz * -1, quat.w);\n"
            "}\n"
            "\n"
            "float4 quatMult(float4 quat1, float4 quat2)\n"
            "{\n"
            "    float3 vec1 = quat1.xyz;\n"
            "    float3 vec2 = quat2.xyz;\n"
            "    float3 qvec = (vec2 * quat1.w) + (vec1 * quat2.w) + cross(vec1, vec2);\n"
            "    float4 qret = float4(0, 0, 0, 0);\n"
            "    qret.x = qvec.x;\n"
            "    qret.y = qvec.y;\n"
            "    qret.z = qvec.z;\n"
            "    qret.w = (quat1.w * quat2.w) - dot(vec1, vec2);\n"
            "    return qret;\n"
            "}\n"
            "\n"
            "float3 quatTransform(float4 quat, float3 vec)\n"
            "{\n"
            "    float4 vq = float4(vec, 0);\n"
            "    float4 q2 = quatConj(quat);\n"
            "    float4 qr = quatMult(quatMult(quat, vq), q2);\n"
            "    return qr.xyz;\n"
            "}\n"
            "\n"
            "float4 quatNew(float3 axis, float degree)\n"
            "{\n"
            "    float sinv = sin(radians(degree / 2.0f));\n"
            "    float cosv = cos(radians(degree / 2.0f));\n"
            "\n"
            "    float4 q = float4(0, 0, 0, 0);\n"
            "    q.x = axis.x * sinv;\n"
            "    q.y = axis.y * sinv;\n"
            "    q.z = axis.z * sinv;\n"
            "    q.w = cosv;\n"
            "    return q;\n"
            "}\n"
            "\n"
            "PSOutput psMain(Output input) {\n"
            "    PSOutput output;\n"
            "    float3 normalColor = tex.Sample(smp, input.texCoord).xyz;\n"
            "    float3 normalVec   = 2.0f * normalColor - 1.0f;\n"
            "    normalVec = normalize(normalVec);\n"
            "\n"
            "    float4 quat = quatNew(input.axis.xyz, input.axis.w);\n"
            "    normalVec = quatTransform(quat, normalVec);\n"
            "    normalVec = normalize(normalVec);\n"
            "\n"
            "    float bright = dot(normalize(float3(1, 1, 0)), normalVec);\n"
            "    bright = max(0.0f, bright);\n"
            "    bright = ((bright * 0.5f) + 0.5f);\n"
            "\n"
            "    float3 vecColor = normalVec.xyz + float3(1, 1, 1);\n"
            "    vecColor *= 0.5;\n"
            "\n"
            "\n"
            "/*\n"
            "    float4 borderCol = borderTex.Sample(borderSmp, input.texCoord);\n"
            "    float border = 1 - step(0.5, borderCol.x);\n"
            "    float4 col = input.color;\n"
            "    col.x *= border;\n"
            "    col.z *= border;\n"
            "*/\n"
            "\n"
            "    float fogStart = 10;\n"
            "    float fogEnd = 200;\n"
            "    float4 fogColor = float4(0, 0, 1, 1);\n"
            "    float distance = length(input.mmpos - input.cameraPosition);\n"
            "    float fogFactor = saturate((distance - fogStart) / (fogEnd - fogStart));\n"
            "\n"
            "    float4 borderCol = borderTex.Sample(borderSmp, input.texCoord);\n"
            "    float border = step(0.5, borderCol.x);\n"
            "    float4 col = input.color;\n"
            "\n"
            "    float borderSize = lerp(0.009, 0.1, fogFactor);\n"
            "    if (input.texCoord.x > (1 - borderSize) || input.texCoord.y > (1 - borderSize) ||\n"
            "        input.texCoord.x < borderSize || input.texCoord.y < borderSize) {\n"
            "        col = float4(0, 1, 0, 1);\n"
            "    }\n"
            "    col = lerp(col, fogColor, fogFactor);\n"
            "\n"
            "    // return float4(vecColor, input.color.w);\n"
            "    output.outPosition = input.mmpos;\n"
            "    output.outNormal = float4(normalVec, 1);\n"
            "    output.outColor = col;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
                Uniform { 0, Uniform::Type::SRV },
                Uniform { 0, Uniform::Type::SRV },
            },
            // cs
            nullptr,
            // csUniforms
            std::vector<Uniform> {
            },
        },
        Program {
            // inputLayout
            Reflect::InputLayout::VertexNormal3D,
            // instanceBufferLayout
            std::vector<Reflect::InstanceBufferType> {
                Reflect::InstanceBufferType::VertexParticle3D,
            },
            // primitiveType
            Reflect::PrimitiveType::Triangles,
            // isWireframe
            false,
            // stencil
            Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
            // vs
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "    float lifetime : LIFETIME;\n"
            "};\n"
            "cbuffer cbuff0 : register(b0) {\n"
            "    matrix modelMatrix;\n"
            "    matrix viewMatrix;\n"
            "    matrix projectionMatrix;\n"
            "}\n"
            "cbuffer cbuff1 : register(b1) { float4 color; }\n"
            "\n"
            "Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float3 offset : OFFSET, float3 velocity : VELOCITY, float lifetime : LIFETIME) {\n"
            "    Output output;\n"
            "    output.svpos = mul(modelMatrix, float4(pos, 1)) + float4(offset, 0);\n"
            "    output.svpos = mul(viewMatrix, output.svpos);\n"
            "    output.svpos = mul(projectionMatrix, output.svpos);\n"
            "    output.wpos = mul(modelMatrix, float4(pos, 1)) + float4(offset, 0);\n"
            "    output.normal = normal;\n"
            "    output.color = color;\n"
            "    output.lifetime = lifetime;\n"
            "    return output;\n"
            "}\n"
            ,
            // vsUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
                Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
            },
            // gs
            nullptr,
            // gsUniforms
            std::vector<Uniform> {
            },
            // ps
            "struct Output {\n"
            "    float4 svpos : SV_POSITION;\n"
            "    float3 wpos : POSITION;\n"
            "    float3 normal : NORMAL;\n"
            "    float4 color : COLOR;\n"
            "    float lifetime : LIFETIME;\n"
            "};\n"
            "struct PSOutput\n"
            "{\n"
            "    float4 outPosition : SV_Target0;\n"
            "    float4 outNormal : SV_Target1;\n"
            "    float4 outColor : SV_Target2;\n"
            "};\n"
            "\n"
            "PSOutput psMain(Output input) : SV_TARGET {\n"
            "    if (input.lifetime <= 0) {\n"
            "        discard;\n"
            "    }\n"
            "    PSOutput output;\n"
            "    output.outPosition = float4(input.wpos, 1);\n"
            "    output.outNormal = float4(input.normal, 1);\n"
            "    output.outColor = input.color;\n"
            "    return output;\n"
            "}\n"
            ,
            // psUniforms
            std::vector<Uniform> {
            },
            // cs
            "struct InstanceData {\n"
            "    float3 offset : OFFSET;\n"
            "    float3 velocity : VELOCITY;\n"
            "    float lifetime : LIFETIME;\n"
            "};\n"
            "\n"
            "RWStructuredBuffer<InstanceData> instanceBuffer : register(u0);\n"
            "\n"
            "cbuffer Constants : register(b0) {\n"
            "    float deltaTime;\n"
            "}\n"
            "\n"
            "[numthreads(256, 1, 1)]\n"
            "void csMain(uint3 dispatchThreadId : SV_DispatchThreadID) {\n"
            "    uint id = dispatchThreadId.x;\n"
            "    InstanceData data = instanceBuffer[id];\n"
            "    data.offset += data.velocity * deltaTime;\n"
            "    data.lifetime = max(0, data.lifetime - deltaTime);\n"
            "    instanceBuffer[id] = data;\n"
            "}\n"
            ,
            // csUniforms
            std::vector<Uniform> {
                Uniform { sizeof(Reflect::UFloat), Uniform::Type::CBV },
                Uniform { Reflect::InstanceBufferSize[static_cast<int32_t>(Reflect::InstanceBufferType::VertexParticle3D)], Uniform::Type::UAV },
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
    class Signature<ProgramTable::Color2D_StencilRead> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Color2D_StencilWrite> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Text2D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Text2D_StencilRead> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Text2D_StencilWrite> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Texture2D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Texture2D_StencilRead> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::Texture2D_StencilWrite> {
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
    class Signature<ProgramTable::MeshLine3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::MeshInstanceColor3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::MeshInstanceTexture3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::TileInstance3D> {
    public:
        static inline void set() { }
    };

    template<>
    class Signature<ProgramTable::ParticleInstance3D> {
    public:
        static inline void set() { }
    };

}
