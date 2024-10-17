#include <Graphics/Buffer.hpp>
#include <Graphics/Constant.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/TileBatch.hpp>
#include <Graphics/VertexTexCoord3D.hpp>
#include <Math/Vector.hpp>
#include <Utils/String.hpp>
#include <vector>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<TileBatch> TileBatch::create(
    const std::shared_ptr<ITileBuffer>& tileBuffer,
    const std::shared_ptr<Texture>& normalTexture,
    float tileSize,
    Style style)
{
    auto tileBatch = std::shared_ptr<TileBatch>(new TileBatch());
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    // shader
    std::unordered_map<std::string, std::string> shaderKeywords;
    shaderKeywords.insert_or_assign("VS_TileDataSize", std::to_string(tileBuffer->getArraySize()));

    tileBatch->m_tileSize = tileSize;
    tileBatch->m_style = style;
    tileBatch->m_shader = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 uv : TEXCOORD;
            float4 color : COLOR;
        	float3 lightTangentDirect : TEXCOORD3;
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
            float3 normalVectorTable[6];
            float3 tangentVectorTable[6];
            float3 binormalVectorTable[6];
            float padding2[10];
        };
        cbuffer cbuff3 : register(b3)
        {
            float4 colorTable[64];
        };

        matrix InvTangentMatrix(
        	float3 tangent,
        	float3 binormal,
        	float3 normal )
        {
        	matrix mat = matrix(float4(tangent, 0.0f ), float4(binormal, 0.0f), float4(normal, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
            return transpose(mat);
        }

        Output vsMain(float3 pos : POSITION, float2 uv : TEXCOORD, uint instanceID : SV_InstanceID) {
            Output output;
            int tileInfo = int(tileData[instanceID].w);
            int tileRotationID = tileInfo % 10;
            int tileColorID = tileInfo / 10;
            matrix tileRotation = rotationMatrixTable[tileRotationID];
            matrix tileTranslate = translateMatrixTable[tileRotationID];
            matrix tileTransform = mul(tileTranslate, tileRotation);
            float3 tileOffset = tileData[instanceID].xyz;

            float4 tmp = float4(mul(tileTransform, float4(pos, 1)) + tileOffset, 1);

            // メッシュのつなぎ目（gap, seamsなどと呼ばれる）が描画されないための対策
            // 隙間なくタイルを敷き詰めているつもりでも、
            // 浮動小数点数演算の誤差によってタイルの間に隙間が生じることがある。
            // これを抑えるために、小数点第二位以下を切り捨てる。
            tmp = round(tmp * 100.0f) / 100.0f;

            tmp = mul(modelMatrix, tmp);
            tmp = mul(viewMatrix, tmp);
            tmp = mul(projectionMatrix, tmp);

            output.svpos = tmp;
            output.color = colorTable[tileColorID];
            output.uv = uv;

            float3 normal = normalVectorTable[tileRotationID];
            float3 tangent = tangentVectorTable[tileRotationID];
            float3 binormal = binormalVectorTable[tileRotationID];
            matrix invTangentMat = InvTangentMatrix(normalize(tangent), normalize(binormal), normalize(normal));
            output.lightTangentDirect = mul(invTangentMat, float4(-normalize(float3(0.0f, 1.0f, 1.0f)), 1.0f));

            return output;
        })"),
                                              shaderKeywords),
        "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 uv : TEXCOORD;
            float4 color : COLOR;
        	float3 lightTangentDirect : TEXCOORD3;
        };

        Texture2D<float4> tex : register(t0);
        SamplerState smp : register(s0);

        float4 psMain(Output input) : SV_TARGET {
            float3 normalColor = tex.Sample(smp, input.uv).xyz;
            float3 normalVec   = 2.0f * normalColor - 1.0f;
            normalVec = normalize(normalVec);

            float3 bright = dot(normalize(input.lightTangentDirect), normalVec);
            bright = max(0.0f, bright);
            bright = ((bright * 0.1f) + 0.9f);

            return float4(bright * input.color.xyz, input.color.w);
        })",
        "psMain");
    // vertex buffer and index buffer
    std::vector<VertexTexCoord3D> vertices;
    std::vector<uint32_t> indices;
    const float half = tileSize / 2.0f;
    const float left = -half;
    const float right = half;
    const float top = half;
    const float bottom = -half;
    vertices.push_back(VertexTexCoord3D(Math::Vector3({ left, bottom, 0 }), Math::Vector2({ 0.0f, 1.0f })));
    vertices.push_back(VertexTexCoord3D(Math::Vector3({ left, top, 0 }), Math::Vector2({ 0.0f, 0.0f })));
    vertices.push_back(VertexTexCoord3D(Math::Vector3({ right, bottom, 0 }), Math::Vector2({ 1.0f, 1.0f })));
    vertices.push_back(VertexTexCoord3D(Math::Vector3({ right, top, 0 }), Math::Vector2({ 1.0f, 0.0f })));
    tileBatch->m_vertexBuffer = Buffer::create();
    tileBatch->m_vertexBuffer->allocate(sizeof(VertexTexCoord3D) * vertices.size());
    tileBatch->m_vertexBuffer->update(vertices.data());
    if (style == Style::Solid) {
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(2);
        indices.emplace_back(1);
        indices.emplace_back(3);
    } else {
        indices.emplace_back(0);
        indices.emplace_back(1);

        indices.emplace_back(0);
        indices.emplace_back(2);

        indices.emplace_back(1);
        indices.emplace_back(3);

        indices.emplace_back(3);
        indices.emplace_back(2);

        if (style == Style::WireframeWithCross) {
            indices.emplace_back(2);
            indices.emplace_back(1);
        }
    }
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
    inputLayout.push_back(
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
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
    psoDesc.PrimitiveTopologyType = style == Style::Solid ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
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
    descTableRange.push_back({});
    descTableRange.at(3).NumDescriptors = 1;
    descTableRange.at(3).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    descTableRange.at(3).BaseShaderRegister = 3;
    descTableRange.at(3).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    descTableRange.push_back({});
    descTableRange.at(4).NumDescriptors = 1;
    descTableRange.at(4).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descTableRange.at(4).BaseShaderRegister = 0;
    descTableRange.at(4).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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
    rootParam.push_back({});
    rootParam.at(4).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParam.at(4).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParam.at(4).DescriptorTable.pDescriptorRanges = &descTableRange.at(4);
    rootParam.at(4).DescriptorTable.NumDescriptorRanges = 1;

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

    tileBatch->m_transformData.translateMatrixTable = getGlobalTranslateMatrixTable(tileSize);
    tileBatch->m_transformData.rotationMatrixTable = getGlobalRotationMatrixTable();
    tileBatch->m_transformData.normalVectorTable = k_normalVectorTable;
    tileBatch->m_transformData.tangentVectorTable = k_tangentVectorTable;
    tileBatch->m_transformData.binormalVectorTable = k_binormalVectorTable;
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
        tileBatch->m_commands.at(i).drawArguments.IndexCountPerInstance = indices.size();
        tileBatch->m_commands.at(i).drawArguments.InstanceCount = 0;
        tileBatch->m_commands.at(i).drawArguments.StartIndexLocation = 0;
        tileBatch->m_commands.at(i).drawArguments.StartInstanceLocation = 0;

        tileBatch->m_commandVisibleTable.push_back(false);
        tileBatch->m_commandIndexTable.push_back(i);
        constBufAddr += tileBatch->m_tileBuffer->getElementSize();
    }
    tileBatch->m_commandBuffer->allocate(sizeof(IndirectCommand) * tileBatch->m_commands.size());
    tileBatch->m_commandBuffer->update(tileBatch->m_commands.data());

    //
    // Descriptor Heap
    //

    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NodeMask = 0;
    descHeapDesc.NumDescriptors = 1;
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&tileBatch->m_descriptorHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }

    D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = tileBatch->m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = Texture::encodeFormat(normalTexture->getFormat());
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    auto texBuff = std::any_cast<ComPtr<ID3D12Resource>>(normalTexture->getID3D12Resource());
    device->CreateShaderResourceView(texBuff.Get(), &srvDesc, heapHandle);
    tileBatch->m_normalTexture = normalTexture;
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

int32_t TileBatch::countSpace() const
{
    return std::count(m_commandVisibleTable.begin(), m_commandVisibleTable.end(), false);
}

bool TileBatch::hasSpace() const
{
    for (bool commandVisible : m_commandVisibleTable) {
        if (!commandVisible) {
            return true;
        }
    }
    return false;
}

void TileBatch::setTiles(int32_t index, const Math::Vector4* tiles, int32_t tileCount)
{
    if (!m_commandVisibleTable.at(index)) {
        throw std::logic_error("rent() is not being called.");
    }
    if (tileCount < 0) {
        throw std::logic_error("count should be positive.");
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

std::array<Math::Matrix, 6> TileBatch::getGlobalTranslateMatrixTable(float tileSize)
{
    const float k_tileHalf = tileSize / 2.0f;
    return {
        // posY
        Math::Matrix::translate(Math::Vector3({ 0.0f, k_tileHalf, 0.0f })),
        // negY
        Math::Matrix::translate(Math::Vector3({ 0.0f, -k_tileHalf, 0.0f })),
        // posX
        Math::Matrix::translate(Math::Vector3({ k_tileHalf, 0.0f, 0.0f })),
        // negX
        Math::Matrix::translate(Math::Vector3({ -k_tileHalf, 0.0f, 0.0f })),
        // posZ
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.0f, k_tileHalf })),
        // negZ
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.0f, -k_tileHalf })),
    };
}

std::array<Math::Matrix, 6> TileBatch::getGlobalRotationMatrixTable()
{
    return {
        // posY
        Math::Matrix::rotateX(-90.0f),
        // negY
        Math::Matrix::rotateX(90.0f),
        // posX
        Math::Matrix::rotateY(90.0f),
        // negX
        Math::Matrix::rotateY(-90.0f),
        // posZ
        Math::Matrix::rotateY(180.0f),
        // negZ
        Math::Matrix(),
    };
}
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

    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    cmdList->SetDescriptorHeaps(1, m_descriptorHeap.GetAddressOf());
    cmdList->SetGraphicsRootDescriptorTable(4, heapHandle);

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();

    bool isWireframe = m_style == Style::Wireframe || m_style == Style::WireframeWithCross;
    cmdList->IASetPrimitiveTopology(isWireframe ? D3D_PRIMITIVE_TOPOLOGY_LINELIST : D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = m_vertexBuffer->getID3D12Resource()->GetGPUVirtualAddress();
    vbView.SizeInBytes = m_vertexBuffer->getSize();
    vbView.StrideInBytes = static_cast<UINT>(sizeof(VertexTexCoord3D));
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
    : m_tileSize()
    , m_shader()
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
    , m_normalTexture()
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