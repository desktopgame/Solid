#include <Graphics/Buffer.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/TileBatch.hpp>
#include <Math/Vector.hpp>
#include <vector>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<TileBatch> TileBatch::create()
{
    auto tileBatch = std::shared_ptr<TileBatch>(new TileBatch());
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    // shader
    tileBatch->m_shader = Shader::compile(R"(
        struct Output {
            float4 svpos : SV_POSITION;
        };
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }

        Output vsMain(float3 pos : POSITION, float4 tileData : POSITION1) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos + tileData.xyz, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
            return output;
        })",
        "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
        };

        float4 psMain(Output input) : SV_TARGET {
            return float4(1,1,1,1);
        })",
        "psMain");
    // vertex buffer and index buffer
    std::vector<Math::Vector3> vertices;
    std::vector<uint32_t> indices;
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.emplace_back(Math::Vector3({ left, bottom, 0 }));
    vertices.emplace_back(Math::Vector3({ left, top, 0 }));
    vertices.emplace_back(Math::Vector3({ right, bottom, 0 }));
    vertices.emplace_back(Math::Vector3({ right, top, 0 }));
    tileBatch->m_vertexBuffer = Buffer::create();
    tileBatch->m_vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
    tileBatch->m_vertexBuffer->update(vertices.data());
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
    tileBatch->m_indexBuffer = Buffer::create();
    tileBatch->m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    tileBatch->m_indexBuffer->update(indices.data());
    tileBatch->m_indexLength = static_cast<int32_t>(indices.size());
    // tile data
    std::vector<Math::Vector4> tileDatas;
    tileDatas.emplace_back(Math::Vector4({ 0, 0, 0, 0 }));
    tileDatas.emplace_back(Math::Vector4({ 1.5f, 0, 0, 0 }));
    tileDatas.emplace_back(Math::Vector4({ -1.5f, 0, 0, 0 }));
    tileBatch->m_tileDataBuffer = Buffer::create();
    tileBatch->m_tileDataBuffer->allocate(sizeof(Math::Vector4) * tileDatas.size());
    tileBatch->m_tileDataBuffer->update(tileDatas.data());
    // input layout
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
    inputLayout.push_back(
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
    inputLayout.push_back(
        { "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
    psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
    psoDesc.InputLayout.NumElements = inputLayout.size();
    // shader
    tileBatch->m_shader->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
    // rasterize
    psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    psoDesc.RasterizerState.MultisampleEnable = false;
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    psoDesc.RasterizerState.DepthClipEnable = true;
    // depth
    psoDesc.DepthStencilState.DepthEnable = true;
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
    rtBlendDesc.BlendEnable = false;
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
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
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
    if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&tileBatch->m_rootSignature)))) {
        throw std::runtime_error("failed CreateRootSignature()");
    }
    psoDesc.pRootSignature = tileBatch->m_rootSignature.Get();
    if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&tileBatch->m_pipelineState)))) {
        throw std::runtime_error("failed CreateGraphicsPipelineState()");
    }
    return tileBatch;
}

TileBatch::~TileBatch()
{
}
// internal
void TileBatch::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
    const std::shared_ptr<Constant> constant)
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

    /*
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
    */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /*
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
    */
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = m_vertexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    vbView.SizeInBytes = m_vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(sizeof(Math::Vector3));
    cmdList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_VERTEX_BUFFER_VIEW tileDataView = {};
    tileDataView.BufferLocation = m_tileDataBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    tileDataView.SizeInBytes = m_tileDataBuffer->getSize();
    tileDataView.StrideInBytes = static_cast<UINT>(sizeof(Math::Vector4));
    cmdList->IASetVertexBuffers(1, 1, &tileDataView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = m_indexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = m_indexBuffer->getSize();
    cmdList->IASetIndexBuffer(&ibView);

    cmdList->DrawIndexedInstanced(m_indexLength, 3, 0, 0, 0);
}
// private
TileBatch::TileBatch()
    : m_shader()
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_tileDataBuffer()
    , m_indexLength()
    , m_pipelineState()
    , m_rootSignature()
{
}
}