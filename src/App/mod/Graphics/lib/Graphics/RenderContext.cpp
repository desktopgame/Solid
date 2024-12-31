#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Metadata.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/VertexNormal3D.hpp>
#include <Graphics/VertexNormalTexCoord3D.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Graphics/VertexTexCoord3D.hpp>
#include <cassert>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
std::vector<std::shared_ptr<RenderContext>> RenderContext::s_table(Metadata::ProgramTable::Count);
// static
static D3D_PRIMITIVE_TOPOLOGY convPrimitiveTopology(Reflect::PrimitiveType primitiveType)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (primitiveType) {
    case Reflect::PrimitiveType::Triangles:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case Reflect::PrimitiveType::Lines:
        return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case Reflect::PrimitiveType::LineStrip:
        return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    }
#pragma clang diagnostic pop
    return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
}
static D3D12_PRIMITIVE_TOPOLOGY_TYPE convPrimitiveTopologyType(Reflect::PrimitiveType primitiveType)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (primitiveType) {
    case Reflect::PrimitiveType::Triangles:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    case Reflect::PrimitiveType::Lines:
    case Reflect::PrimitiveType::LineStrip:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    }
#pragma clang diagnostic pop
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
}
// public
std::shared_ptr<RenderContext> RenderContext::get(Metadata::ProgramTable entry)
{
    return s_table.at(entry);
}
// internal
void RenderContext::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength)
{
    render(cmdList,
        ub,
        vertexBuffer,
        indexBuffer,
        indexLength,
        nullptr,
        0,
        0);
}

void RenderContext::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength,
    const std::vector<std::shared_ptr<Buffer>>& instanceBuffers,
    int32_t instanceCount)
{
    render(cmdList,
        ub,
        vertexBuffer,
        indexBuffer,
        indexLength,
        instanceBuffers.data(),
        static_cast<int32_t>(instanceBuffers.size()),
        instanceCount);
}

void RenderContext::initialize()
{
    for (int32_t i = 0; i < Metadata::ProgramTable::Count; i++) {
        auto rc = std::shared_ptr<RenderContext>(new RenderContext());
        rc->m_entry = (Metadata::ProgramTable)i;
        rc->init((Metadata::ProgramTable)i);
        s_table[i] = rc;
    }
}

void RenderContext::destroy()
{
    s_table.clear();
}
// private
RenderContext::RenderContext()
{
}

void RenderContext::init(Metadata::ProgramTable entry)
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    const int32_t i = static_cast<int32_t>(entry);
    const Metadata::Program& program = Metadata::k_programs.at(i);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    // input layout
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
    bool depthTest = false;
    bool depthWrite = false;
    bool blendEnable = false;
    bool useGBuffer = false;
    switch (program.inputLayout) {
    case Reflect::InputLayout::Vertex2D:
        blendEnable = true;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        break;
    case Reflect::InputLayout::VertexTexCoord2D:
        blendEnable = true;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        break;
    case Reflect::InputLayout::Vertex3D:
        depthTest = true;
        depthWrite = true;
        useGBuffer = true;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        break;
    case Reflect::InputLayout::VertexNormal3D:
        depthTest = true;
        depthWrite = true;
        useGBuffer = true;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        break;
    case Reflect::InputLayout::VertexTexCoord3D:
        depthTest = true;
        depthWrite = true;
        useGBuffer = true;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        break;
    case Reflect::InputLayout::VertexNormalTexCoord3D:
        depthTest = true;
        depthWrite = true;
        useGBuffer = true;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        break;
    }
    for (int32_t i = 0; i < static_cast<int32_t>(program.instanceBufferLayout.size()); i++) {
        Reflect::InstanceBufferType instBufType = program.instanceBufferLayout.at(i);

        switch (instBufType) {
        case Reflect::InstanceBufferType::Vector2:
            inputLayout.push_back(
                { "INSTANCE", (UINT)i, DXGI_FORMAT_R32G32_FLOAT, (UINT)(i + 1),
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            break;
        case Reflect::InstanceBufferType::Vector3:
            inputLayout.push_back(
                { "INSTANCE", (UINT)i, DXGI_FORMAT_R32G32B32_FLOAT, (UINT)(i + 1),
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            break;
        case Reflect::InstanceBufferType::Vector4:
            inputLayout.push_back(
                { "INSTANCE", (UINT)i, DXGI_FORMAT_R32G32B32A32_FLOAT, (UINT)(i + 1),
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            break;
        case Reflect::InstanceBufferType::VertexParticle2D:
            inputLayout.push_back(
                { "OFFSET", (UINT)i, DXGI_FORMAT_R32G32_FLOAT, (UINT)(i + 1),
                    0,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            inputLayout.push_back(
                { "VELOCITY", (UINT)i, DXGI_FORMAT_R32G32_FLOAT, (UINT)(i + 1),
                    8,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            break;
        case Reflect::InstanceBufferType::VertexParticle3D:
            inputLayout.push_back(
                { "OFFSET", (UINT)i, DXGI_FORMAT_R32G32B32_FLOAT, (UINT)(i + 1),
                    0,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            inputLayout.push_back(
                { "VELOCITY", (UINT)i, DXGI_FORMAT_R32G32B32_FLOAT, (UINT)(i + 1),
                    12,
                    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
            break;
        case Reflect::InstanceBufferType::Count:
            // unreachable
            break;
        }
    }
    psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
    psoDesc.InputLayout.NumElements = inputLayout.size();
    // shader
    char vsName[16];
    ::memset(vsName, '\0', 16);
    ::sprintf(vsName, "RC[%d]_VS", i);
    m_vShader = Shader::compile("vs_5_0", "vsMain", program.vsCode, vsName);
    m_vShader->getD3D12_SHADER_BYTECODE(psoDesc.VS);

    if (program.gsCode != nullptr) {
        char gsName[16];
        ::memset(gsName, '\0', 16);
        ::sprintf(gsName, "RC[%d]_GS", i);
        m_gShader = Shader::compile("gs_5_0", "gsMain", program.gsCode, gsName);
        m_gShader->getD3D12_SHADER_BYTECODE(psoDesc.GS);
    }

    char psName[16];
    ::memset(psName, '\0', 16);
    ::sprintf(psName, "RC[%d]_PS", i);
    m_pShader = Shader::compile("ps_5_0", "psMain", program.psCode, psName);
    m_pShader->getD3D12_SHADER_BYTECODE(psoDesc.PS);

    if (program.csCode != nullptr) {
        char csName[16];
        ::memset(csName, '\0', 16);
        ::sprintf(csName, "RC[%d]_CS", i);
        m_cShader = Shader::compile("cs_5_0", "csMain", program.csCode, csName);
    }
    // rasterize
    psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psoDesc.RasterizerState.MultisampleEnable = false;
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.FillMode = program.isWireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
    psoDesc.RasterizerState.DepthClipEnable = true;
    // depth
    psoDesc.DepthStencilState.DepthEnable = depthTest;
    psoDesc.DepthStencilState.DepthWriteMask = depthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // blend
    psoDesc.BlendState.AlphaToCoverageEnable = false;
    psoDesc.BlendState.IndependentBlendEnable = false;
    D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
    rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    rtBlendDesc.BlendEnable = blendEnable;
    rtBlendDesc.LogicOpEnable = false;
    rtBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    rtBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    rtBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    rtBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    rtBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    rtBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
    if (useGBuffer) {
        psoDesc.BlendState.RenderTarget[0] = rtBlendDesc;
        psoDesc.BlendState.RenderTarget[1] = rtBlendDesc;
        psoDesc.BlendState.RenderTarget[2] = rtBlendDesc;
    } else {
        psoDesc.BlendState.RenderTarget[0] = rtBlendDesc;
    }
    psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    psoDesc.PrimitiveTopologyType = convPrimitiveTopologyType(program.primitiveType);
    if (useGBuffer) {
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        psoDesc.RTVFormats[2] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        psoDesc.NumRenderTargets = 3;
    } else {
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.NumRenderTargets = 1;
    }
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    // root signature
    std::vector<D3D12_DESCRIPTOR_RANGE> descTableRange;
    int32_t offset = 0;
    for (int32_t vsUniform = 0; vsUniform < program.vsUniforms.size(); vsUniform++) {
        descTableRange.push_back({});
        descTableRange.at(offset + vsUniform).NumDescriptors = 1;
        descTableRange.at(offset + vsUniform).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(offset + vsUniform).BaseShaderRegister = vsUniform;
        descTableRange.at(offset + vsUniform).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    offset = static_cast<int32_t>(program.vsUniforms.size());
    for (int32_t gsUniform = 0; gsUniform < program.gsUniforms.size(); gsUniform++) {
        descTableRange.push_back({});
        descTableRange.at(offset + gsUniform).NumDescriptors = 1;
        descTableRange.at(offset + gsUniform).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(offset + gsUniform).BaseShaderRegister = gsUniform;
        descTableRange.at(offset + gsUniform).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    int32_t psSRV = 0;
    int32_t psCBV = 0;
    offset = static_cast<int32_t>(program.vsUniforms.size()) + static_cast<int32_t>(program.gsUniforms.size());
    for (int32_t psUniform = 0; psUniform < program.psUniforms.size(); psUniform++) {
        Metadata::Uniform u = program.psUniforms.at(psUniform);
        bool isShaderResource = u.type == Metadata::Uniform::Type::SRV;
        assert(u.type != Metadata::Uniform::Type::UAV);
        descTableRange.push_back({});
        descTableRange.at(offset + psUniform).NumDescriptors = 1;
        descTableRange.at(offset + psUniform).RangeType = isShaderResource ? D3D12_DESCRIPTOR_RANGE_TYPE_SRV : D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(offset + psUniform).BaseShaderRegister = isShaderResource ? psSRV : psCBV;
        descTableRange.at(offset + psUniform).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        (isShaderResource ? psSRV : psCBV)++;
    }

    std::vector<D3D12_ROOT_PARAMETER> rootParam;
    offset = 0;
    for (int32_t vsUniform = 0; vsUniform < program.vsUniforms.size(); vsUniform++) {
        rootParam.push_back({});
        rootParam.at(offset + vsUniform).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(offset + vsUniform).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        rootParam.at(offset + vsUniform).DescriptorTable.pDescriptorRanges = &descTableRange.at(offset + vsUniform);
        rootParam.at(offset + vsUniform).DescriptorTable.NumDescriptorRanges = 1;
    }
    offset = static_cast<int32_t>(program.vsUniforms.size());
    for (int32_t gsUniform = 0; gsUniform < program.gsUniforms.size(); gsUniform++) {
        rootParam.push_back({});
        rootParam.at(offset + gsUniform).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(offset + gsUniform).ShaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY;
        rootParam.at(offset + gsUniform).DescriptorTable.pDescriptorRanges = &descTableRange.at(offset + gsUniform);
        rootParam.at(offset + gsUniform).DescriptorTable.NumDescriptorRanges = 1;
    }
    offset = static_cast<int32_t>(program.vsUniforms.size()) + static_cast<int32_t>(program.gsUniforms.size());
    for (int32_t psUniform = 0; psUniform < program.psUniforms.size(); psUniform++) {
        rootParam.push_back({});
        rootParam.at(offset + psUniform).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(offset + psUniform).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(offset + psUniform).DescriptorTable.pDescriptorRanges = &descTableRange.at(offset + psUniform);
        rootParam.at(offset + psUniform).DescriptorTable.NumDescriptorRanges = 1;
    }

    std::vector<D3D12_STATIC_SAMPLER_DESC> samplerDescs(psSRV);
    for (int32_t sampler = 0; sampler < psSRV; sampler++) {
        D3D12_STATIC_SAMPLER_DESC& samplerDesc = samplerDescs.at(sampler);
        samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
        samplerDesc.MinLOD = 0.0f;
        samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        samplerDesc.ShaderRegister = sampler;
    }
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.pParameters = rootParam.data();
    rootSignatureDesc.NumParameters = rootParam.size();
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pStaticSamplers = samplerDescs.data();
    rootSignatureDesc.NumStaticSamplers = psSRV;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob))) {
        throw std::runtime_error("failed D3D12SerializeRootSignature()");
    }
    if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)))) {
        throw std::runtime_error("failed CreateRootSignature()");
    }
    psoDesc.pRootSignature = m_rootSignature.Get();
    if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)))) {
        throw std::runtime_error("failed CreateGraphicsPipelineState()");
    }
    // compute pipeline state
    if (m_cShader) {
        // root signature
        std::vector<D3D12_DESCRIPTOR_RANGE> computeDescTableRange;
        int32_t csSRV = 0;
        int32_t csCBV = 0;
        int32_t csUAV = 0;
        for (int32_t csUniform = 0; csUniform < static_cast<int32_t>(program.csUniforms.size()); csUniform++) {
            Metadata::Uniform u = program.csUniforms.at(csUniform);
            computeDescTableRange.push_back({});
            computeDescTableRange.at(csUniform).NumDescriptors = 1;
            computeDescTableRange.at(csUniform).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            switch (u.type) {
            case Metadata::Uniform::Type::CBV:
                computeDescTableRange.at(csUniform).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                computeDescTableRange.at(csUniform).BaseShaderRegister = csCBV;
                csCBV++;
                break;
            case Metadata::Uniform::Type::SRV:
                computeDescTableRange.at(csUniform).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                computeDescTableRange.at(csUniform).BaseShaderRegister = csSRV;
                csSRV++;
                break;
            case Metadata::Uniform::Type::UAV:
                computeDescTableRange.at(csUniform).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                computeDescTableRange.at(csUniform).BaseShaderRegister = csUAV;
                csUAV++;
                break;
            }
        }

        std::vector<D3D12_ROOT_PARAMETER> computeRootParam;
        for (int32_t csUniform = 0; csUniform < static_cast<int32_t>(program.csUniforms.size()); csUniform++) {
            computeRootParam.push_back({});
            computeRootParam.at(csUniform).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            computeRootParam.at(csUniform).ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            computeRootParam.at(csUniform).DescriptorTable.pDescriptorRanges = &computeDescTableRange.at(csUniform);
            computeRootParam.at(csUniform).DescriptorTable.NumDescriptorRanges = 1;
        }

        std::vector<D3D12_STATIC_SAMPLER_DESC> computeSamplerDescs(csSRV);
        for (int32_t sampler = 0; sampler < csSRV; sampler++) {
            D3D12_STATIC_SAMPLER_DESC& samplerDesc = computeSamplerDescs.at(sampler);
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            samplerDesc.ShaderRegister = sampler;
        }

        D3D12_ROOT_SIGNATURE_DESC computeRootSignatureDesc = {};
        computeRootSignatureDesc.pParameters = computeRootParam.data();
        computeRootSignatureDesc.NumParameters = computeRootParam.size();
        computeRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        computeRootSignatureDesc.pStaticSamplers = computeSamplerDescs.data();
        computeRootSignatureDesc.NumStaticSamplers = csSRV;
        ComPtr<ID3DBlob> computeRootSigBlob = nullptr;
        ComPtr<ID3DBlob> computeErrorBlob = nullptr;
        if (FAILED(D3D12SerializeRootSignature(&computeRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &computeRootSigBlob, &computeErrorBlob))) {
            throw std::runtime_error("failed D3D12SerializeRootSignature()");
        }
        if (FAILED(device->CreateRootSignature(0, computeRootSigBlob->GetBufferPointer(), computeRootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_computeRootSignature)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }

        D3D12_COMPUTE_PIPELINE_STATE_DESC computePsoDesc {};
        m_cShader->getD3D12_SHADER_BYTECODE(computePsoDesc.CS);
        computePsoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
        computePsoDesc.NodeMask = 0;
        computePsoDesc.pRootSignature = m_computeRootSignature.Get();

        if (FAILED(device->CreateComputePipelineState(&computePsoDesc, IID_PPV_ARGS(&m_computePipelineState)))) {
            throw std::runtime_error("failed CreateComputePipelineState()");
        }
    }
}

void RenderContext::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength,
    const std::shared_ptr<Buffer>* instanceBuffers,
    int32_t instanceBufferCount,
    int32_t instanceCount)
{
    const Metadata::Program& program = Metadata::k_programs.at(m_entry);
    if (program.instanceBufferLayout.size() > 0 && instanceBufferCount == 0) {
        throw std::logic_error("missing instance buffer.");
    }

    cmdList->SetPipelineState(m_pipelineState.Get());
    cmdList->SetGraphicsRootSignature(m_rootSignature.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    auto descriptorHeap = ub->getID3D12DescriptorHeap();
    uint64_t incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    cmdList->SetDescriptorHeaps(1, descriptorHeap.GetAddressOf());

    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    for (int32_t i = 0; i < program.vsUniforms.size() + program.gsUniforms.size() + program.psUniforms.size(); i++) {
        cmdList->SetGraphicsRootDescriptorTable(i, heapHandle);
        heapHandle.ptr += incrementSize;
    }
    cmdList->IASetPrimitiveTopology(convPrimitiveTopology(program.primitiveType));

    uint32_t stride = 0;
    switch (program.inputLayout) {
    case Reflect::InputLayout::Vertex2D:
        stride = sizeof(Math::Vector2);
        break;
    case Reflect::InputLayout::VertexTexCoord2D:
        stride = sizeof(VertexTexCoord2D);
        break;
    case Reflect::InputLayout::Vertex3D:
        stride = sizeof(Math::Vector3);
        break;
    case Reflect::InputLayout::VertexNormal3D:
        stride = sizeof(VertexNormal3D);
        break;
    case Reflect::InputLayout::VertexTexCoord3D:
        stride = sizeof(VertexTexCoord3D);
        break;
    case Reflect::InputLayout::VertexNormalTexCoord3D:
        stride = sizeof(VertexNormalTexCoord3D);
        break;
    }
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = vertexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    vbView.SizeInBytes = vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(stride);
    cmdList->IASetVertexBuffers(0, 1, &vbView);

    for (int32_t i = 0; i < instanceBufferCount; i++) {
        std::shared_ptr<Buffer> instBuffer = instanceBuffers[i];
        Reflect::InstanceBufferType instBufType = program.instanceBufferLayout.at(i);

        D3D12_VERTEX_BUFFER_VIEW instView = {};
        instView.BufferLocation = instBuffer->getID3D12Resource()->GetGPUVirtualAddress();
        instView.SizeInBytes = instBuffer->getSize();
        instView.StrideInBytes = static_cast<UINT>(Reflect::InstanceBufferSize[static_cast<int32_t>(instBufType)]);
        cmdList->IASetVertexBuffers(i + 1, 1, &instView);
    }

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = indexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = indexBuffer->getSize();
    cmdList->IASetIndexBuffer(&ibView);

    if (instanceBufferCount > 0) {
        cmdList->DrawIndexedInstanced(indexLength, instanceCount, 0, 0, 0);
    } else {
        cmdList->DrawIndexedInstanced(indexLength, 1, 0, 0, 0);
    }
}
}