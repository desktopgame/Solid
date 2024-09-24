#include <Graphics/Buffer.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/TileBatch.hpp>
#include <Math/Vector.hpp>
#include <Utils/String.hpp>
#include <vector>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<TileBatch> TileBatch::create(const std::shared_ptr<ITileBuffer> tileBuffer)
{
    auto tileBatch = std::shared_ptr<TileBatch>(new TileBatch());
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    // shader
    std::unordered_map<std::string, std::string> shaderKeywords;
    shaderKeywords.insert_or_assign("VS_TileDataSize", std::to_string(tileBuffer->getArraySize()));
    shaderKeywords.insert_or_assign("VS_TileOffsetScale", std::to_string(s_tileSize));

    tileBatch->m_shader = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float4 color : COLOR;
        };
        cbuffer cbuff0 : register(b0)
        {
            float4 tileData[${VS_TileDataSize}];
            matrix modelMatrix;
        };
        cbuffer cbuff1 : register(b1)
        {
            matrix viewMatrix;
            matrix projectionMatrix;
            float4 padding[8];
        };
        cbuffer cbuff2 : register(b2)
        {
            matrix translateMatrixTable[6];
            matrix rotationMatrixTable[6];
        };
        cbuffer cbuff3 : register(b3)
        {
            float4 colorTable[64];
        };

        Output vsMain(float3 pos : POSITION, uint instanceID : SV_InstanceID) {
            Output output;
            int tileInfo = int(tileData[instanceID].w);
            int tileRotationID = tileInfo % 10;
            int tileColorID = tileInfo / 10;
            matrix tileRotation = rotationMatrixTable[tileRotationID];
            matrix tileTranslate = translateMatrixTable[tileRotationID];
            matrix tileTransform = mul(tileTranslate, tileRotation);
            float3 tileOffset = tileData[instanceID].xyz * ${VS_TileOffsetScale};
            matrix mvpMatrix = mul(mul(projectionMatrix, viewMatrix), modelMatrix);
            output.svpos = mul(mvpMatrix, float4(mul(tileTransform, float4(pos, 1)) + tileOffset, 1));
            output.color = colorTable[tileColorID];
            return output;
        })"),
                                              shaderKeywords),
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
    const float half = s_tileHalf;
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
    rootParam.push_back({});
    rootParam.at(1).ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParam.at(1).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(1).Constants.RegisterSpace = 0;
    rootParam.at(1).Constants.ShaderRegister = 1;
    rootParam.push_back({});
    rootParam.at(2).ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParam.at(2).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(2).Constants.RegisterSpace = 0;
    rootParam.at(2).Constants.ShaderRegister = 2;
    rootParam.push_back({});
    rootParam.at(3).ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParam.at(3).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParam.at(3).Constants.RegisterSpace = 0;
    rootParam.at(3).Constants.ShaderRegister = 3;

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
    tileBatch->m_tileBuffer = tileBuffer;
    tileBatch->m_constantBuffer = Buffer::create();
    tileBatch->m_constantBuffer->allocate(tileBatch->m_tileBuffer->getElementSize() * tileBatch->m_tileBuffer->getElementCount());
    tileBatch->m_constantBuffer->update(tileBatch->m_tileBuffer->getElementPtr());

    // command signature
    std::vector<D3D12_INDIRECT_ARGUMENT_DESC> argumentDescs;
    argumentDescs.push_back({});
    argumentDescs.at(0).Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    argumentDescs.at(0).ConstantBufferView.RootParameterIndex = 0;
    argumentDescs.push_back({});
    argumentDescs.at(1).Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    argumentDescs.at(1).ConstantBufferView.RootParameterIndex = 1;
    argumentDescs.push_back({});
    argumentDescs.at(2).Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    argumentDescs.at(2).ConstantBufferView.RootParameterIndex = 2;
    argumentDescs.push_back({});
    argumentDescs.at(3).Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
    argumentDescs.at(3).ConstantBufferView.RootParameterIndex = 3;
    argumentDescs.push_back({});
    argumentDescs.at(4).Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

    D3D12_COMMAND_SIGNATURE_DESC commandSignatureDesc = {};
    commandSignatureDesc.pArgumentDescs = argumentDescs.data();
    commandSignatureDesc.NumArgumentDescs = argumentDescs.size();
    commandSignatureDesc.ByteStride = sizeof(IndirectCommand);

    if (FAILED(device->CreateCommandSignature(&commandSignatureDesc, tileBatch->m_rootSignature.Get(), IID_PPV_ARGS(&tileBatch->m_commandSignature)))) {
        throw std::runtime_error("failed CreateCommandSignature()");
    }
    // global buffer
    tileBatch->m_cameraBuffer = Buffer::create();
    tileBatch->m_cameraBuffer->allocate(sizeof(CameraData));
    tileBatch->m_cameraBuffer->update(&tileBatch->m_cameraData);

    tileBatch->m_transformData.translateMatrixTable = s_translateMatrixTable;
    tileBatch->m_transformData.rotationMatrixTable = s_rotationMatrixTable;
    tileBatch->m_transformBuffer = Buffer::create();
    tileBatch->m_transformBuffer->allocate(sizeof(TransformData));
    tileBatch->m_transformBuffer->update(&tileBatch->m_transformData);

    tileBatch->m_colorData.colorTable = k_colorTable;
    tileBatch->m_colorBuffer = Buffer::create();
    tileBatch->m_colorBuffer->allocate(sizeof(ColorData));
    tileBatch->m_colorBuffer->update(&tileBatch->m_colorData);
    // command buffer
    tileBatch->m_commands.reserve(tileBatch->m_tileBuffer->getElementCount());
    tileBatch->m_commandVisibleTable.reserve(tileBatch->m_tileBuffer->getElementCount());
    tileBatch->m_commandIndexTable.reserve(tileBatch->m_tileBuffer->getElementCount());
    tileBatch->m_commandBuffer = Buffer::create();
    auto constBufAddr = tileBatch->m_constantBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    auto cameraBufAddr = tileBatch->m_cameraBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    auto transformBufAddr = tileBatch->m_transformBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    auto colorBufAddr = tileBatch->m_colorBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    for (int32_t i = 0; i < tileBatch->m_tileBuffer->getElementCount(); i++) {
        tileBatch->m_commands.push_back(IndirectCommand {});
        tileBatch->m_commands.at(i).instanceBuffer = constBufAddr;
        tileBatch->m_commands.at(i).cameraBuffer = cameraBufAddr;
        tileBatch->m_commands.at(i).transformBuffer = transformBufAddr;
        tileBatch->m_commands.at(i).colorBuffer = colorBufAddr;
        tileBatch->m_commands.at(i).drawArguments.IndexCountPerInstance = 6;
        tileBatch->m_commands.at(i).drawArguments.InstanceCount = 0;
        tileBatch->m_commands.at(i).drawArguments.StartIndexLocation = 0;
        tileBatch->m_commands.at(i).drawArguments.StartInstanceLocation = 0;

        tileBatch->m_commandVisibleTable.push_back(false);
        tileBatch->m_commandIndexTable.push_back(i);
        constBufAddr += tileBatch->m_tileBuffer->getElementSize();
    }
    tileBatch->m_commandBuffer->allocate(sizeof(IndirectCommand) * tileBatch->m_commands.size());
    tileBatch->m_commandBuffer->update(tileBatch->m_commands.data());
    return tileBatch;
}

TileBatch::~TileBatch()
{
}

int32_t TileBatch::rent()
{
    for (int32_t i = 0; i < m_commandVisibleTable.size(); i++) {
        if (!m_commandVisibleTable.at(i)) {
            m_commandVisibleTable.at(i) = true;
            m_shouldCompact = true;
            m_shouldCommandCopy = true;
            return i;
        }
    }
    return -1;
}

void TileBatch::release(int32_t index)
{
    if (!m_commandVisibleTable.at(index)) {
        throw std::logic_error("already released.");
    }
    m_commandVisibleTable.at(index) = false;
    m_shouldCompact = true;
    m_shouldCommandCopy = true;
}

void TileBatch::setTiles(int32_t index, const Math::Vector4* tiles, int32_t tileCount)
{
    if (!m_commandVisibleTable.at(index)) {
        throw std::logic_error("rent() is not being called.");
    }
    Math::Vector4* dst = m_tileBuffer->getArrayAt(index);
    ::memcpy(dst, tiles, sizeof(Math::Vector4) * tileCount);
    m_shouldConstantCopy = true;

    m_commands.at(m_commandIndexTable.at(index)).drawArguments.InstanceCount = tileCount;
    m_shouldCommandCopy = true;
}
const Math::Vector4* TileBatch::getTiles(int32_t index) const
{
    if (!m_commandVisibleTable.at(index)) {
        throw std::logic_error("rent() is not being called.");
    }
    return m_tileBuffer->getArrayAt(index);
}
int32_t TileBatch::getTileSize() const
{
    return m_tileBuffer->getArraySize();
}

void TileBatch::setMatrix(int32_t index, const Math::Matrix& matrix)
{
    if (!m_commandVisibleTable.at(index)) {
        throw std::logic_error("rent() is not being called.");
    }
    m_tileBuffer->getMatrixAt(index) = matrix;
    m_shouldConstantCopy = true;
}
Math::Matrix TileBatch::getMatrix(int32_t index) const
{
    if (!m_commandVisibleTable.at(index)) {
        throw std::logic_error("rent() is not being called.");
    }
    return m_tileBuffer->getMatrixAt(index);
}

void TileBatch::setGlobalViewMatrix(const Math::Matrix& matrix)
{
    m_cameraData.viewMatrix = matrix;
    m_shouldCameraCopy = true;
}
Math::Matrix TileBatch::getGlobalViewMatrix() const { return m_cameraData.viewMatrix; }

void TileBatch::setGlobalProjectionMatrix(const Math::Matrix& matrix)
{
    m_cameraData.projectionMatrix = matrix;
    m_shouldCameraCopy = true;
}
Math::Matrix TileBatch::getGlobalProjectionMatrix() const { return m_cameraData.projectionMatrix; }
// internal
void TileBatch::render(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    int32_t visibleCount = std::count(m_commandVisibleTable.begin(), m_commandVisibleTable.end(), true);
    if (visibleCount == 0) {
        return;
    }
    // compaction
    if (m_shouldCompact) {
        auto constBufAddr = m_constantBuffer->getID3D12Resource()->GetGPUVirtualAddress();
        int32_t cursor = 0;
        for (int32_t i = 0; i < m_tileBuffer->getElementCount(); i++) {
            if (m_commandVisibleTable.at(i)) {
                if (cursor != i) {
                    m_commands.at(cursor) = m_commands.at(i);
                    m_commands.at(cursor).instanceBuffer = constBufAddr + (i * m_tileBuffer->getElementSize());
                    m_commandIndexTable.at(i) = cursor;
                }
                cursor++;
                if (cursor == visibleCount) {
                    break;
                }
            }
        }
        m_shouldCompact = false;
    }
    if (m_shouldCommandCopy) {
        m_commandBuffer->update(m_commands.data());
        m_shouldCommandCopy = false;
    }
    if (m_shouldConstantCopy) {
        m_constantBuffer->update(m_tileBuffer->getElementPtr());
        m_shouldConstantCopy = false;
    }
    if (m_shouldCameraCopy) {
        m_cameraBuffer->update(&m_cameraData);
        m_shouldCameraCopy = false;
    }
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
        m_commandSignature.Get(), visibleCount, m_commandBuffer->getID3D12Resource().Get(),
        0, nullptr, 0);
}
// private
TileBatch::TileBatch()
    : m_shader()
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_constantBuffer()
    , m_commandBuffer()
    , m_tileBuffer()
    , m_commandVisibleTable()
    , m_commandIndexTable()
    , m_cameraData()
    , m_cameraBuffer()
    , m_transformData()
    , m_transformBuffer()
    , m_colorData()
    , m_colorBuffer()
    , m_indexLength()
    , m_shouldCompact()
    , m_shouldCommandCopy()
    , m_shouldConstantCopy()
    , m_shouldCameraCopy()
    , m_commands()
    , m_pipelineState()
    , m_rootSignature()
    , m_commandSignature()
{
}
}