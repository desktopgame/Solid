#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/VertexNormal3D.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Graphics/VertexTexCoord3D.hpp>
#include <Math/Matrix.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// static
static D3D_PRIMITIVE_TOPOLOGY convPrimitiveTopology(PrimitiveType primitiveType)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (primitiveType) {
    case PrimitiveType::Triangles:
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case PrimitiveType::LineStrip:
        return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    }
#pragma clang diagnostic pop
    return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
}
static D3D12_PRIMITIVE_TOPOLOGY_TYPE convPrimitiveTopologyType(PrimitiveType primitiveType)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (primitiveType) {
    case PrimitiveType::Triangles:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    case PrimitiveType::LineStrip:
        return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    }
#pragma clang diagnostic pop
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
}
// public
std::shared_ptr<PipelineStateObject> PipelineStateObject::create(
    const std::shared_ptr<Shader>& shader,
    Constant::Layout constantLayout,
    PrimitiveType primitiveType,
    int32_t vertexComponent,
    bool isUsingNormal,
    bool isUsingTexCoord)
{
    auto pso = std::shared_ptr<PipelineStateObject>(new PipelineStateObject());
    pso->m_shader = shader;
    pso->m_constantLayout = constantLayout;
    pso->m_primitiveType = primitiveType;
    pso->m_vertexComponent = vertexComponent;
    pso->m_isUsingNormal = isUsingNormal;
    pso->m_isUsingTexCoord = isUsingTexCoord;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    // input layout
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
    if (pso->m_vertexComponent == 3) {
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        if (pso->m_isUsingNormal) {
            inputLayout.push_back(
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        }
    } else {
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        if (pso->m_isUsingNormal) {
            throw std::runtime_error(" isUsingNormal can use only by 3d vertex.");
        }
    }
    if (pso->m_isUsingTexCoord) {
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
    }
    psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
    psoDesc.InputLayout.NumElements = inputLayout.size();
    // shader
    pso->m_shader->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
    // rasterize
    psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psoDesc.RasterizerState.MultisampleEnable = false;
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    psoDesc.RasterizerState.DepthClipEnable = true;
    // depth
    psoDesc.DepthStencilState.DepthEnable = pso->m_vertexComponent == 3;
    psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // blend
    psoDesc.BlendState.AlphaToCoverageEnable = false;
    psoDesc.BlendState.IndependentBlendEnable = false;
    D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
    rtBlendDesc.BlendEnable = false;
    rtBlendDesc.LogicOpEnable = false;
    rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    rtBlendDesc.BlendEnable = pso->m_vertexComponent == 2;
    rtBlendDesc.LogicOpEnable = false;
    rtBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    rtBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    rtBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    rtBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    rtBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    rtBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
    psoDesc.BlendState.RenderTarget[0] = rtBlendDesc;
    psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    psoDesc.PrimitiveTopologyType = convPrimitiveTopologyType(pso->m_primitiveType);
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    // root signature
    std::vector<D3D12_DESCRIPTOR_RANGE> descTableRange;
    descTableRange.push_back({});
    descTableRange.at(0).NumDescriptors = 1;
    descTableRange.at(0).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descTableRange.at(0).BaseShaderRegister = 0;
    descTableRange.at(0).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    descTableRange.push_back({});
    descTableRange.at(1).NumDescriptors = 1;
    descTableRange.at(1).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descTableRange.at(1).BaseShaderRegister = 1;
    descTableRange.at(1).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    descTableRange.push_back({});
    descTableRange.at(2).NumDescriptors = 1;
    descTableRange.at(2).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descTableRange.at(2).BaseShaderRegister = 2;
    descTableRange.at(2).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    if (constantLayout.useTexture()) {
        descTableRange.push_back({});
        descTableRange.at(3).NumDescriptors = 1;
        descTableRange.at(3).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(3).BaseShaderRegister = 0;
        descTableRange.at(3).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        if (constantLayout.useColor()) {
            descTableRange.push_back({});
            descTableRange.at(4).NumDescriptors = 1;
            descTableRange.at(4).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(4).BaseShaderRegister = 3;
            descTableRange.at(4).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        } else if (constantLayout.useLightDirection()) {
            descTableRange.push_back({});
            descTableRange.at(4).NumDescriptors = 1;
            descTableRange.at(4).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(4).BaseShaderRegister = 3;
            descTableRange.at(4).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }
    } else if (constantLayout.useColor()) {
        descTableRange.push_back({});
        descTableRange.at(3).NumDescriptors = 1;
        descTableRange.at(3).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(3).BaseShaderRegister = 3;
        descTableRange.at(3).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        if (constantLayout.useLightDirection()) {
            descTableRange.push_back({});
            descTableRange.at(4).NumDescriptors = 1;
            descTableRange.at(4).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(4).BaseShaderRegister = 4;
            descTableRange.at(4).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }
    }
    std::vector<D3D12_ROOT_PARAMETER> rootParam;
    rootParam.push_back({});
    rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(0).DescriptorTable.pDescriptorRanges = &descTableRange.at(0);
    rootParam.at(0).DescriptorTable.NumDescriptorRanges = 1;
    rootParam.push_back({});
    rootParam.at(1).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.at(1).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(1).DescriptorTable.pDescriptorRanges = &descTableRange.at(1);
    rootParam.at(1).DescriptorTable.NumDescriptorRanges = 1;
    rootParam.push_back({});
    rootParam.at(2).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.at(2).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(2).DescriptorTable.pDescriptorRanges = &descTableRange.at(2);
    rootParam.at(2).DescriptorTable.NumDescriptorRanges = 1;

    if (constantLayout.useTexture()) {
        rootParam.push_back({});
        rootParam.at(3).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(3).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(3).DescriptorTable.pDescriptorRanges = &descTableRange.at(3);
        rootParam.at(3).DescriptorTable.NumDescriptorRanges = 1;

        if (constantLayout.useColor()) {
            rootParam.push_back({});
            rootParam.at(4).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.at(4).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            rootParam.at(4).DescriptorTable.pDescriptorRanges = &descTableRange.at(4);
            rootParam.at(4).DescriptorTable.NumDescriptorRanges = 1;
        } else if (constantLayout.useLightDirection()) {
            rootParam.push_back({});
            rootParam.at(4).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.at(4).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            rootParam.at(4).DescriptorTable.pDescriptorRanges = &descTableRange.at(4);
            rootParam.at(4).DescriptorTable.NumDescriptorRanges = 1;
        }
    } else if (constantLayout.useColor()) {
        rootParam.push_back({});
        rootParam.at(3).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(3).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        rootParam.at(3).DescriptorTable.pDescriptorRanges = &descTableRange.at(3);
        rootParam.at(3).DescriptorTable.NumDescriptorRanges = 1;

        if (constantLayout.useLightDirection()) {
            rootParam.push_back({});
            rootParam.at(4).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.at(4).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            rootParam.at(4).DescriptorTable.pDescriptorRanges = &descTableRange.at(4);
            rootParam.at(4).DescriptorTable.NumDescriptorRanges = 1;
        }
    }
    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.pParameters = rootParam.data();
    rootSignatureDesc.NumParameters = rootParam.size();
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob))) {
        throw std::runtime_error("failed D3D12SerializeRootSignature()");
    }
    if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pso->m_rootSignature)))) {
        throw std::runtime_error("failed CreateRootSignature()");
    }
    psoDesc.pRootSignature = pso->m_rootSignature.Get();
    if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso->m_pipelineState)))) {
        throw std::runtime_error("failed CreateGraphicsPipelineState()");
    }
    return pso;
}
PipelineStateObject::~PipelineStateObject()
{
}
// internal
void PipelineStateObject::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
    const std::shared_ptr<Constant> constant,
    const std::shared_ptr<Buffer>& vertexBuffer,
    const std::shared_ptr<Buffer>& indexBuffer,
    int32_t indexLength)
{
    cmdList->SetPipelineState(m_pipelineState.Get());
    cmdList->SetGraphicsRootSignature(m_rootSignature.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    auto descriptorHeap = constant->getID3D12DescriptorHeap();
    uint64_t incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    cmdList->SetDescriptorHeaps(1, descriptorHeap.GetAddressOf());

    cmdList->SetGraphicsRootDescriptorTable(0, heapHandle);

    heapHandle.ptr += incrementSize;
    cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);

    heapHandle.ptr += incrementSize;
    cmdList->SetGraphicsRootDescriptorTable(2, heapHandle);

    if (m_constantLayout != constant->getLayout()) {
        throw std::runtime_error("missmatch interfaces.");
    }
    if (m_constantLayout.useTexture()) {
        if (!constant->getTexture()) {
            throw std::runtime_error("texture missing.");
        }
        heapHandle.ptr += incrementSize;
        cmdList->SetGraphicsRootDescriptorTable(3, heapHandle);
        if (m_constantLayout.useColor()) {
            heapHandle.ptr += incrementSize;
            cmdList->SetGraphicsRootDescriptorTable(4, heapHandle);
        }
    } else if (m_constantLayout.useColor()) {
        heapHandle.ptr += incrementSize;
        cmdList->SetGraphicsRootDescriptorTable(3, heapHandle);

        if (m_constantLayout.useLightDirection()) {
            heapHandle.ptr += incrementSize;
            cmdList->SetGraphicsRootDescriptorTable(4, heapHandle);
        }
    }
    cmdList->IASetPrimitiveTopology(convPrimitiveTopology(m_primitiveType));

    uint32_t stride = 0;
    if (m_vertexComponent == 2) {
        if (m_isUsingTexCoord) {
            stride = sizeof(VertexTexCoord2D);
        } else {
            stride = sizeof(Math::Vector2);
        }
    } else if (m_vertexComponent == 3) {
        if (m_isUsingTexCoord) {
            stride = sizeof(VertexTexCoord3D);
        } else {
            if (m_isUsingNormal) {
                stride = sizeof(VertexNormal3D);
            } else {
                stride = sizeof(Math::Vector3);
            }
        }
    }
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = vertexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    vbView.SizeInBytes = vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(stride);
    cmdList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = indexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = indexBuffer->getSize();
    cmdList->IASetIndexBuffer(&ibView);

    cmdList->DrawIndexedInstanced(indexLength, 1, 0, 0, 0);
}
// private
PipelineStateObject::PipelineStateObject()
    : m_shader()
    , m_constantLayout()
    , m_primitiveType()
    , m_vertexComponent(0)
    , m_isUsingNormal(false)
    , m_isUsingTexCoord(false)
    , m_pipelineState()
    , m_rootSignature()
{
}
}