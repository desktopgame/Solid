#include <Graphics/Buffer.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Screen.hpp>
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
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0)
        {
            float4 tileData[16];
            matrix mvpMatrix;
        };

        static const matrix rotationTable[6] = {
            // posY
            matrix(
            1, 0, 0, 0,
            0, cos(radians(-90)), -sin(radians(-90)), 0,
            0, sin(radians(-90)), cos(radians(-90)), 0,
            0, 0, 0, 1
            ),
            // negY
            matrix(
            1, 0, 0, 0,
            0, cos(radians(90)), -sin(radians(90)), 0,
            0, sin(radians(90)), cos(radians(90)), 0,
            0, 0, 0, 1
            ),
            // posX
            matrix(
            cos(radians(90)), 0, sin(radians(90)), 0,
            0, 1, 0, 0,
            -sin(radians(90)), 0, cos(radians(90)), 0,
            0, 0, 0, 1
            ),
            // negX
            matrix(
            cos(radians(-90)), 0, sin(radians(-90)), 0,
            0, 1, 0, 0,
            -sin(radians(-90)), 0, cos(radians(-90)), 0,
            0, 0, 0, 1
            ),
            // posZ
            matrix(
            cos(radians(180)), 0, sin(radians(180)), 0,
            0, 1, 0, 0,
            -sin(radians(180)), 0, cos(radians(180)), 0,
            0, 0, 0, 1
            ),
            // negZ
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
            )
        };
        static const matrix translateTable[6] = {
            // posY
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0.5, 0, 1
            ),
            // negY
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, -0.5, 0, 1
            ),
            // posX
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0.5, 0, 0, 1
            ),
            // negX
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -0.5, 0, 0, 1
            ),
            // posZ
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0.5, 1
            ),
            // negZ
            matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, -0.5, 1
            )
        };

        Output vsMain(float3 pos : POSITION, uint instanceID : SV_InstanceID) {
            Output output;
            int tileInfo = int(tileData[instanceID].w);
            int tileRotationID = tileInfo % 10;
            matrix tileRotation = transpose(rotationTable[tileRotationID]);
            matrix tileTranslate = transpose(translateTable[tileRotationID]);
            matrix tileTransform = mul(tileTranslate, tileRotation);
            float3 tileOffset = tileData[instanceID].xyz;
            output.svpos = mul(mvpMatrix, float4(mul(tileTransform, float4(pos, 1)) + tileOffset, 1));
            output.color = float4(float(instanceID) / 12.0, 0, 0, 1);
            return output;
        })",
        "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };

        float4 psMain(Output input) : SV_TARGET {
            return input.color;
        })",
        "psMain");
    // vertex buffer and index buffer
    std::vector<Math::Vector3> vertices;
    std::vector<uint32_t> indices;
    const float half = 0.5f;
    const float left = -half;
    const float right = half;
    const float top = half;
    const float bottom = -half;
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
    // input layout
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
    inputLayout.push_back(
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
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
    std::vector<D3D12_ROOT_PARAMETER> rootParam;
    rootParam.push_back({});
    rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(0).Constants.RegisterSpace = 0;
    rootParam.at(0).Constants.ShaderRegister = 0;

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
    // constant buffer
    tileBatch->m_constantBuffer = Buffer::create();
    for (int32_t i = 0; i < 10; i++) {
        float fx = static_cast<float>(i) * 2.0f;
        tileBatch->m_constantBufferData.push_back(ConstantData {});
        tileBatch->m_constantBufferData.at(i).tiles = {
            Math::Vector4({ -2.0f, 0, fx, 0 }),
            Math::Vector4({ -2.0f, 0, fx, 1 }),
            Math::Vector4({ -2.0f, 0, fx, 2 }),
            Math::Vector4({ -2.0f, 0, fx, 3 }),
            Math::Vector4({ -2.0f, 0, fx, 4 }),
            Math::Vector4({ -2.0f, 0, fx, 5 }),

            Math::Vector4({ 2.0f, 0, fx, 0 }),
            Math::Vector4({ 2.0f, 0, fx, 1 }),
            Math::Vector4({ 2.0f, 0, fx, 2 }),
            Math::Vector4({ 2.0f, 0, fx, 3 }),
            Math::Vector4({ 2.0f, 0, fx, 4 }),
            Math::Vector4({ 2.0f, 0, fx, 5 }),

            Math::Vector4({ 0, 0, 0, 0 }),
            Math::Vector4({ 0, 0, 0, 0 }),
            Math::Vector4({ 0, 0, 0, 0 }),
            Math::Vector4({ 0, 0, 0, 0 }),
        };
        tileBatch->m_constantBufferData.at(i).matrix = Math::Matrix() * Math::Matrix::lookAt(Math::Vector3({ 0, 0.5f, -2 }), Math::Vector3({ 0, 0, 0 }), Math::Vector3({ 0, 1, 0 })) * Math::Matrix::perspective(90.0f, Screen::getAspectRatio(), 0.1f, 100.0f);
    }
    tileBatch->m_constantBuffer->allocate(sizeof(ConstantData) * tileBatch->m_constantBufferData.size());
    tileBatch->m_constantBuffer->update(tileBatch->m_constantBufferData.data());
    // command signature
    std::vector<D3D12_INDIRECT_ARGUMENT_DESC> argumentDescs;
    argumentDescs.push_back({});
    argumentDescs.at(0).Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    argumentDescs.at(0).ConstantBufferView.RootParameterIndex = 0;
    argumentDescs.push_back({});
    argumentDescs.at(1).Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

    D3D12_COMMAND_SIGNATURE_DESC commandSignatureDesc = {};
    commandSignatureDesc.pArgumentDescs = argumentDescs.data();
    commandSignatureDesc.NumArgumentDescs = argumentDescs.size();
    commandSignatureDesc.ByteStride = sizeof(IndirectCommand);

    if (FAILED(device->CreateCommandSignature(&commandSignatureDesc, tileBatch->m_rootSignature.Get(), IID_PPV_ARGS(&tileBatch->m_commandSignature)))) {
        throw std::runtime_error("failed CreateCommandSignature()");
    }
    // command buffer
    tileBatch->m_commandBuffer = Buffer::create();
    auto constBufAddr = tileBatch->m_constantBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    std::vector<IndirectCommand> commands;
    for (int32_t i = 0; i < 10; i++) {
        commands.push_back(IndirectCommand {});
        commands.at(i).cbv = constBufAddr;
        commands.at(i).drawArguments.IndexCountPerInstance = 6;
        commands.at(i).drawArguments.InstanceCount = 12;
        commands.at(i).drawArguments.StartIndexLocation = 0;
        commands.at(i).drawArguments.StartInstanceLocation = 0;

        constBufAddr += sizeof(ConstantData);
    }
    tileBatch->m_commandBuffer->allocate(sizeof(IndirectCommand) * commands.size());
    tileBatch->m_commandBuffer->update(commands.data());
    return tileBatch;
}

TileBatch::~TileBatch()
{
}
// internal
void TileBatch::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    cmdList->SetPipelineState(m_pipelineState.Get());
    cmdList->SetGraphicsRootSignature(m_rootSignature.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();

    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = m_vertexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    vbView.SizeInBytes = m_vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(sizeof(Math::Vector3));
    cmdList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = m_indexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = m_indexBuffer->getSize();
    cmdList->IASetIndexBuffer(&ibView);

    cmdList->ExecuteIndirect(
        m_commandSignature.Get(), 10 /* command count */, m_commandBuffer->getID3D12Resource().Get(),
        0, nullptr, 0);
}
// private
TileBatch::TileBatch()
    : m_shader()
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_constantBuffer()
    , m_commandBuffer()
    , m_indexLength()
    , m_constantBufferData()
    , m_pipelineState()
    , m_rootSignature()
    , m_commandSignature()
{
}
}