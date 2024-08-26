#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/PipelineStateObject.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
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
    RenderInterface renderInterface,
    PrimitiveType primitiveType,
    int32_t vertexComponent,
    bool usingTexCoord)
{
    auto pso = std::shared_ptr<PipelineStateObject>(new PipelineStateObject());
    pso->m_shader = shader;
    pso->m_renderInterface = renderInterface;
    pso->m_primitiveType = primitiveType;
    pso->m_vertexComponent = vertexComponent;
    pso->m_isUsingTexCoord = usingTexCoord;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    // input layout
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
    if (pso->m_vertexComponent == 3) {
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
    } else {
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
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
    psoDesc.VS.pShaderBytecode = pso->m_shader->getVertexShaderBlob()->GetBufferPointer();
    psoDesc.VS.BytecodeLength = pso->m_shader->getVertexShaderBlob()->GetBufferSize();
    psoDesc.PS.pShaderBytecode = pso->m_shader->getPixelShaderBlob()->GetBufferPointer();
    psoDesc.PS.BytecodeLength = pso->m_shader->getPixelShaderBlob()->GetBufferSize();
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
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
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
    if (renderInterface.useTexture()) {
        descTableRange.push_back({});
        descTableRange.at(1).NumDescriptors = 1;
        descTableRange.at(1).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(1).BaseShaderRegister = 0;
        descTableRange.at(1).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        if (renderInterface.useColor()) {
            descTableRange.push_back({});
            descTableRange.at(2).NumDescriptors = 1;
            descTableRange.at(2).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(2).BaseShaderRegister = 1;
            descTableRange.at(2).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }
    } else if (renderInterface.useColor()) {
        descTableRange.push_back({});
        descTableRange.at(1).NumDescriptors = 1;
        descTableRange.at(1).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(1).BaseShaderRegister = 1;
        descTableRange.at(1).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    std::vector<D3D12_ROOT_PARAMETER> rootParam;
    rootParam.push_back({});
    rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(0).DescriptorTable.pDescriptorRanges = &descTableRange.at(0);
    rootParam.at(0).DescriptorTable.NumDescriptorRanges = 1;

    if (renderInterface.useTexture()) {
        rootParam.push_back({});
        rootParam.at(1).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(1).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(1).DescriptorTable.pDescriptorRanges = &descTableRange.at(1);
        rootParam.at(1).DescriptorTable.NumDescriptorRanges = 1;

        if (renderInterface.useColor()) {
            rootParam.push_back({});
            rootParam.at(2).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.at(2).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            rootParam.at(2).DescriptorTable.pDescriptorRanges = &descTableRange.at(2);
            rootParam.at(2).DescriptorTable.NumDescriptorRanges = 1;
        }
    } else if (renderInterface.useColor()) {
        rootParam.push_back({});
        rootParam.at(1).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(1).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        rootParam.at(1).DescriptorTable.pDescriptorRanges = &descTableRange.at(1);
        rootParam.at(1).DescriptorTable.NumDescriptorRanges = 1;
    }
    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
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

std::shared_ptr<Shader> PipelineStateObject::getShader() const { return m_shader; }
RenderInterface PipelineStateObject::getRenderInterface() const { return m_renderInterface; }
PrimitiveType PipelineStateObject::getPrimitiveType() const { return m_primitiveType; }
int32_t PipelineStateObject::getVertexComponent() const { return m_vertexComponent; }
bool PipelineStateObject::isUsingTexCoord() const { return m_isUsingTexCoord; }
// internal
void PipelineStateObject::command(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, const std::shared_ptr<RenderParameter> renderParameter)
{
    cmdList->SetPipelineState(m_pipelineState.Get());
    cmdList->SetGraphicsRootSignature(m_rootSignature.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    auto descriptorHeap = renderParameter->getID3D12DescriptorHeap();
    uint64_t incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    cmdList->SetDescriptorHeaps(1, descriptorHeap.GetAddressOf());
    cmdList->SetGraphicsRootDescriptorTable(0, heapHandle);
    if (m_renderInterface != renderParameter->getInterface()) {
        throw std::runtime_error("missmatch interfaces.");
    }
    if (m_renderInterface.useTexture()) {
        if (!renderParameter->getTexture()) {
            throw std::runtime_error("texture missing.");
        }
        heapHandle.ptr += incrementSize;
        cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);
        if (m_renderInterface.useColor()) {
            heapHandle.ptr += incrementSize;
            cmdList->SetGraphicsRootDescriptorTable(2, heapHandle);
        }
    } else if (m_renderInterface.useColor()) {
        heapHandle.ptr += incrementSize;
        cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);
    }
    cmdList->IASetPrimitiveTopology(convPrimitiveTopology(m_primitiveType));
}
// private
PipelineStateObject::PipelineStateObject()
    : m_shader()
    , m_renderInterface()
    , m_primitiveType()
    , m_vertexComponent(0)
    , m_isUsingTexCoord(false)
    , m_pipelineState()
    , m_rootSignature()
{
}
}