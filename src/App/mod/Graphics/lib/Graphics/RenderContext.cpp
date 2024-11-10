#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Shader.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
std::vector<std::shared_ptr<RenderContext>> RenderContext::s_table(Metadata::ProgramTable::Count);
// public
std::shared_ptr<RenderContext> RenderContext::get(Metadata::ProgramTable entry)
{
    return s_table.at(entry);
}
// private
void RenderContext::initialize()
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    for (int32_t i = 0; i < Metadata::ProgramTable::Count; i++) {
        auto rc = std::shared_ptr<RenderContext>(new RenderContext());
        rc->m_entry = (Metadata::ProgramTable)i;
        s_table.emplace_back(rc);

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        // input layout
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
        bool depthTest = false;
        bool depthWrite = false;
        bool blendEnable = false;
        bool useGBuffer = false;
        switch (Metadata::k_programs.at(i).inputLayout) {
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
        psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
        psoDesc.InputLayout.NumElements = inputLayout.size();
        // shader
        rc->m_shader = Shader::compile(Metadata::k_programs.at(i).vsCode, "vsMain", Metadata::k_programs.at(i).psCode, "psMain");
        rc->m_shader->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
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
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // TODO: Metadataから指定
        if (useGBuffer) {
            psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
            psoDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
            psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
            psoDesc.NumRenderTargets = 3;
        } else {
            psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            psoDesc.NumRenderTargets = 1;
        }
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;
        // root signature
        std::vector<D3D12_DESCRIPTOR_RANGE> descTableRange;
        for (int32_t vsUniform = 0; vsUniform < Metadata::k_programs.at(i).vsUniforms.size(); vsUniform++) {
            descTableRange.push_back({});
            descTableRange.at(vsUniform).NumDescriptors = 1;
            descTableRange.at(vsUniform).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(vsUniform).BaseShaderRegister = vsUniform;
            descTableRange.at(vsUniform).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }
        int32_t psSRV = 0;
        int32_t psCBV = 0;
        for (int32_t psUniform = 0; psUniform < Metadata::k_programs.at(i).psUniforms.size(); psUniform++) {
            int32_t offset = static_cast<int32_t>(Metadata::k_programs.at(i).vsUniforms.size());
            Metadata::Uniform u = Metadata::k_programs.at(i).psUniforms.at(psUniform);
            descTableRange.push_back({});
            descTableRange.at(offset + psUniform).NumDescriptors = 1;
            descTableRange.at(offset + psUniform).RangeType = u.isShaderResource ? D3D12_DESCRIPTOR_RANGE_TYPE_SRV : D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(offset + psUniform).BaseShaderRegister = u.isShaderResource ? psSRV : psCBV;
            descTableRange.at(offset + psUniform).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            (u.isShaderResource ? psSRV : psCBV)++;
        }
        std::vector<D3D12_ROOT_PARAMETER> rootParam;
        for (int32_t vsUniform = 0; vsUniform < Metadata::k_programs.at(i).vsUniforms.size(); vsUniform++) {
            rootParam.push_back({});
            rootParam.at(vsUniform).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.at(vsUniform).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            rootParam.at(vsUniform).DescriptorTable.pDescriptorRanges = &descTableRange.at(vsUniform);
            rootParam.at(vsUniform).DescriptorTable.NumDescriptorRanges = 1;
        }
        for (int32_t psUniform = 0; psUniform < Metadata::k_programs.at(i).psUniforms.size(); psUniform++) {
            int32_t offset = static_cast<int32_t>(Metadata::k_programs.at(i).vsUniforms.size());
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
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rc->m_rootSignature)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = rc->m_rootSignature.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&rc->m_pipelineState)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
    }
}

void RenderContext::destroy()
{
    s_table.clear();
}

RenderContext::RenderContext()
{
}
}