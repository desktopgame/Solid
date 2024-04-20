#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Internal/Pso.hpp>
#include <Graphics/Shader.hpp>
#include <Math/Matrix.hpp>
#include <d3d12.h>

namespace Lib::Graphics::Internal {
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
// Impl
class Pso::Impl {
public:
    explicit Impl() = default;
    ComPtr<ID3D12PipelineState> pipelineState;
    ComPtr<ID3D12RootSignature> rootSignature;
    ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    ComPtr<ID3D12Resource> matrixBuff;
    ComPtr<ID3D12Resource> colorBuff;

private:
};
// public
std::shared_ptr<Pso> Pso::create(
    const std::shared_ptr<Shader>& shader,
    const std::shared_ptr<RenderParameter>& renderParameter,
    PrimitiveType primitiveType,
    int32_t vertexComponent,
    bool usingTexCoord)
{
    auto pso = std::shared_ptr<Pso>(new Pso());
    pso->m_shader = shader;
    pso->m_renderParameter = renderParameter;
    pso->m_primitiveType = primitiveType;
    pso->m_vertexComponent = vertexComponent;
    pso->m_isUsingTexCoord = usingTexCoord;

    auto device = std::any_cast<ComPtr<ID3D12Device>>(Engine::getInstance()->getDevice()->getHandle());

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
    psoDesc.VS.pShaderBytecode = pso->m_shader->getVertexProgram().getPointer();
    psoDesc.VS.BytecodeLength = pso->m_shader->getVertexProgram().getSize();
    psoDesc.PS.pShaderBytecode = pso->m_shader->getPixelProgram().getPointer();
    psoDesc.PS.BytecodeLength = pso->m_shader->getPixelProgram().getSize();
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
    if (pso->m_renderParameter->useTexture()) {
        descTableRange.push_back({});
        descTableRange.at(1).NumDescriptors = 1;
        descTableRange.at(1).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(1).BaseShaderRegister = 0;
        descTableRange.at(1).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        if (pso->m_renderParameter->useColor()) {
            descTableRange.push_back({});
            descTableRange.at(2).NumDescriptors = 1;
            descTableRange.at(2).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTableRange.at(2).BaseShaderRegister = 1;
            descTableRange.at(2).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }
    } else if (pso->m_renderParameter->useColor()) {
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

    if (pso->m_renderParameter->useTexture()) {
        rootParam.push_back({});
        rootParam.at(1).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(1).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(1).DescriptorTable.pDescriptorRanges = &descTableRange.at(1);
        rootParam.at(1).DescriptorTable.NumDescriptorRanges = 1;

        if (pso->m_renderParameter->useColor()) {
            rootParam.push_back({});
            rootParam.at(2).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParam.at(2).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            rootParam.at(2).DescriptorTable.pDescriptorRanges = &descTableRange.at(2);
            rootParam.at(2).DescriptorTable.NumDescriptorRanges = 1;
        }
    } else if (pso->m_renderParameter->useColor()) {
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
    if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pso->m_impl->rootSignature)))) {
        throw std::runtime_error("failed CreateRootSignature()");
    }
    psoDesc.pRootSignature = pso->m_impl->rootSignature.Get();
    if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso->m_impl->pipelineState)))) {
        throw std::runtime_error("failed CreateGraphicsPipelineState()");
    }
    // descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NodeMask = 0;
    descHeapDesc.NumDescriptors = 1;
    if (pso->m_renderParameter->useTexture()) {
        descHeapDesc.NumDescriptors++;
    }
    if (pso->m_renderParameter->useColor()) {
        descHeapDesc.NumDescriptors++;
    }
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&pso->m_impl->descriptorHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-temporary"
    // transform
    if (!pso->m_impl->matrixBuff) {

        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC resDesc = {};
        resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resDesc.Width = (sizeof(Math::Matrix) + 0xff) & ~0xff;
        resDesc.Height = 1;
        resDesc.DepthOrArraySize = 1;
        resDesc.MipLevels = 1;
        resDesc.Format = DXGI_FORMAT_UNKNOWN;
        resDesc.SampleDesc.Count = 1;
        resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &resDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&pso->m_impl->matrixBuff)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
    }
    // color
    if (pso->m_renderParameter->useColor()) {
        if (!pso->m_impl->colorBuff) {
            D3D12_HEAP_PROPERTIES heapProps = {};
            heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            D3D12_RESOURCE_DESC resDesc = {};
            resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            resDesc.Width = (sizeof(Math::Vector4) + 0xff) & ~0xff;
            resDesc.Height = 1;
            resDesc.DepthOrArraySize = 1;
            resDesc.MipLevels = 1;
            resDesc.Format = DXGI_FORMAT_UNKNOWN;
            resDesc.SampleDesc.Count = 1;
            resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            if (FAILED(device->CreateCommittedResource(
                    &heapProps,
                    D3D12_HEAP_FLAG_NONE,
                    &resDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&pso->m_impl->colorBuff)))) {
                throw std::runtime_error("failed CreateCommittedResource()");
            }
        }
    }
    pso->update();
    pso->m_isDirty = false;
    // constant buffer
    auto basicHeapHandle = pso->m_impl->descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    // constant buffer(matrix)
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvMatrixDesc = {};
    cbvMatrixDesc.BufferLocation = pso->m_impl->matrixBuff->GetGPUVirtualAddress();
    cbvMatrixDesc.SizeInBytes = pso->m_impl->matrixBuff->GetDesc().Width;
    device->CreateConstantBufferView(&cbvMatrixDesc, basicHeapHandle);
    // constant buffer(color)
    if (pso->m_renderParameter->useTexture()) {
        basicHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        /*
        auto texBuff = std::static_pointer_cast<DxTexture>(m_shaderParameter->getTexture())->getResource();
        device->CreateShaderResourceView(texBuff.Get(), &srvDesc, basicHeapHandle);

        if (m_shaderParameter->useColor()) {
            basicHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
            cbvColorDesc.BufferLocation = m_colorBuff->GetGPUVirtualAddress();
            cbvColorDesc.SizeInBytes = m_colorBuff->GetDesc().Width;
            device->CreateConstantBufferView(&cbvColorDesc, basicHeapHandle);
        }
        */
    } else if (pso->m_renderParameter->useColor()) {
        basicHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
        cbvColorDesc.BufferLocation = pso->m_impl->colorBuff->GetGPUVirtualAddress();
        cbvColorDesc.SizeInBytes = pso->m_impl->colorBuff->GetDesc().Width;
        device->CreateConstantBufferView(&cbvColorDesc, basicHeapHandle);
    }
#pragma clang diagnostic pop
    // m_texture = m_shaderParameter->getTexture();
    return pso;
}
Pso::~Pso()
{
}
void Pso::update()
{
    // transfrom
    {
        void* mapMatrix = nullptr;
        if (FAILED(m_impl->matrixBuff->Map(0, nullptr, (void**)&mapMatrix))) {
            throw std::runtime_error("failed Map()");
        } else {
            Math::Matrix matrix = m_renderParameter->getTransform();
            ::memcpy(mapMatrix, matrix.data(), sizeof(Math::Matrix));
            m_impl->matrixBuff->Unmap(0, nullptr);
        }
    }
    // color
    if (m_renderParameter->useColor()) {
        void* mapColor = nullptr;
        if (FAILED(m_impl->colorBuff->Map(0, nullptr, (void**)&mapColor))) {
            throw std::runtime_error("failed Map()");
        } else {
            Math::Vector4 color = m_renderParameter->getColor();
            ::memcpy(mapColor, color.data(), sizeof(Math::Vector4));
            m_impl->colorBuff->Unmap(0, nullptr);
        }
    }
    m_isDirty = true;
}

void Pso::command(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    cmdList->SetPipelineState(m_impl->pipelineState.Get());
    cmdList->SetGraphicsRootSignature(m_impl->rootSignature.Get());

    auto device = std::any_cast<ComPtr<ID3D12Device>>(Engine::getInstance()->getDevice()->getHandle());
    uint64_t incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = m_impl->descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    cmdList->SetDescriptorHeaps(1, m_impl->descriptorHeap.GetAddressOf());
    cmdList->SetGraphicsRootDescriptorTable(0, heapHandle);
    if (m_renderParameter->useTexture()) {
        heapHandle.ptr += incrementSize;
        cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);
        if (m_renderParameter->useColor()) {
            heapHandle.ptr += incrementSize;
            cmdList->SetGraphicsRootDescriptorTable(2, heapHandle);
        }
    } else if (m_renderParameter->useColor()) {
        heapHandle.ptr += incrementSize;
        cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);
    }
    cmdList->IASetPrimitiveTopology(convPrimitiveTopology(m_primitiveType));
}
// private
Pso::Pso()
    : m_shader()
    , m_renderParameter()
    , m_primitiveType()
    , m_vertexComponent(0)
    , m_isUsingTexCoord(false)
    , m_isDirty(false)
    , m_impl(std::make_shared<Impl>())
{
}
}