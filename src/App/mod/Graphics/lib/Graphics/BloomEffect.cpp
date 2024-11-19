#include <Graphics/BloomEffect.hpp>
#include <Graphics/Buffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/GlobalLight.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Utils/String.hpp>
#include <stdexcept>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;

std::shared_ptr<Shader> BloomEffect::s_shader;
Microsoft::WRL::ComPtr<ID3D12PipelineState> BloomEffect::s_pipelineState;
Microsoft::WRL::ComPtr<ID3D12RootSignature> BloomEffect::s_rootSignature;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> BloomEffect::s_descriptorHeap;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_vertexBuffer;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_indexBuffer;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_constantBuffer;

std::shared_ptr<Shader> BloomEffect::s_shader2;
Microsoft::WRL::ComPtr<ID3D12PipelineState> BloomEffect::s_pipelineState2;
Microsoft::WRL::ComPtr<ID3D12RootSignature> BloomEffect::s_rootSignature2;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> BloomEffect::s_descriptorHeap2;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_vertexBuffer2;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_indexBuffer2;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_constantBuffer2;

std::shared_ptr<Shader> BloomEffect::s_shader3;
Microsoft::WRL::ComPtr<ID3D12PipelineState> BloomEffect::s_pipelineState3;
Microsoft::WRL::ComPtr<ID3D12RootSignature> BloomEffect::s_rootSignature3;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> BloomEffect::s_descriptorHeap3;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_vertexBuffer3;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_indexBuffer3;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_constantBuffer3;

std::shared_ptr<Shader> BloomEffect::s_shader4;
Microsoft::WRL::ComPtr<ID3D12PipelineState> BloomEffect::s_pipelineState4;
Microsoft::WRL::ComPtr<ID3D12RootSignature> BloomEffect::s_rootSignature4;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> BloomEffect::s_descriptorHeap4;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_vertexBuffer4;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_indexBuffer4;
Microsoft::WRL::ComPtr<ID3D12Resource> BloomEffect::s_constantBuffer4;
// public
// internal
void BloomEffect::draw1(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    commandList->SetPipelineState(s_pipelineState.Get());
    commandList->SetGraphicsRootSignature(s_rootSignature.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    commandList->SetDescriptorHeaps(1, s_descriptorHeap.GetAddressOf());
    commandList->SetGraphicsRootDescriptorTable(0, heapHandle);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = s_vertexBuffer->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(VertexTexCoord2D) * 4;
    vbView.StrideInBytes = static_cast<UINT>(sizeof(VertexTexCoord2D));
    commandList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = s_indexBuffer->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = sizeof(uint32_t) * 6;
    commandList->IASetIndexBuffer(&ibView);

    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void BloomEffect::draw2(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    commandList->SetPipelineState(s_pipelineState2.Get());
    commandList->SetGraphicsRootSignature(s_rootSignature2.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap2->GetGPUDescriptorHandleForHeapStart();
    commandList->SetDescriptorHeaps(1, s_descriptorHeap2.GetAddressOf());
    commandList->SetGraphicsRootDescriptorTable(0, heapHandle);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = s_vertexBuffer2->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(VertexTexCoord2D) * 4;
    vbView.StrideInBytes = static_cast<UINT>(sizeof(VertexTexCoord2D));
    commandList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = s_indexBuffer2->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = sizeof(uint32_t) * 6;
    commandList->IASetIndexBuffer(&ibView);

    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void BloomEffect::draw3(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    commandList->SetPipelineState(s_pipelineState3.Get());
    commandList->SetGraphicsRootSignature(s_rootSignature3.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap3->GetGPUDescriptorHandleForHeapStart();
    commandList->SetDescriptorHeaps(1, s_descriptorHeap3.GetAddressOf());
    commandList->SetGraphicsRootDescriptorTable(0, heapHandle);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = s_vertexBuffer3->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(VertexTexCoord2D) * 4;
    vbView.StrideInBytes = static_cast<UINT>(sizeof(VertexTexCoord2D));
    commandList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = s_indexBuffer3->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = sizeof(uint32_t) * 6;
    commandList->IASetIndexBuffer(&ibView);

    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void BloomEffect::draw4(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    commandList->SetPipelineState(s_pipelineState4.Get());
    commandList->SetGraphicsRootSignature(s_rootSignature4.Get());

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap4->GetGPUDescriptorHandleForHeapStart();
    commandList->SetDescriptorHeaps(1, s_descriptorHeap4.GetAddressOf());
    commandList->SetGraphicsRootDescriptorTable(0, heapHandle);
    heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    commandList->SetGraphicsRootDescriptorTable(1, heapHandle);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    vbView.BufferLocation = s_vertexBuffer4->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(VertexTexCoord2D) * 4;
    vbView.StrideInBytes = static_cast<UINT>(sizeof(VertexTexCoord2D));
    commandList->IASetVertexBuffers(0, 1, &vbView);

    D3D12_INDEX_BUFFER_VIEW ibView = {};
    ibView.BufferLocation = s_indexBuffer4->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R32_UINT;
    ibView.SizeInBytes = sizeof(uint32_t) * 6;
    commandList->IASetIndexBuffer(&ibView);

    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void BloomEffect::initialize(
    const Microsoft::WRL::ComPtr<ID3D12Device>& device,
    const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures)
{
    // 高輝度成分を抜き出し1番に書き込む
    {

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        // input layout
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
        psoDesc.InputLayout.NumElements = inputLayout.size();
        // shader
        std::unordered_map<std::string, std::string> shaderKeywords;
        s_shader = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {
            Output output;
            output.svpos = float4(pos, 0, 1);
            output.texCoord = texCoord;
            return output;
        })"),
                                       shaderKeywords),
            "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Texture2D<float4> srcTex : register(t0);
        SamplerState srcSmp : register(s0);

        float4 psMain(Output input) : SV_TARGET {
            float4 src = srcTex.Sample(srcSmp, input.texCoord);
            float luminance = dot(src.rgb, float3(0.2126, 0.7152, 0.0722));
            if (luminance > 0.4)
            {
                return src;
            }
            else
            {
                return float4(0, 0, 0, 1);
            }
        })",
            "psMain");
        s_shader->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
        // vertex buffer and index buffer
        std::vector<VertexTexCoord2D> vertices;
        std::vector<uint32_t> indices;
        const float half = 1.0f;
        const float left = -half;
        const float right = half;
        const float top = half;
        const float bottom = -half;
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));

        D3D12_HEAP_PROPERTIES vHeapProps = {};
        vHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        vHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC vResDesc = {};
        vResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vResDesc.Width = sizeof(VertexTexCoord2D) * 4;
        vResDesc.Height = 1;
        vResDesc.DepthOrArraySize = 1;
        vResDesc.MipLevels = 1;
        vResDesc.Format = DXGI_FORMAT_UNKNOWN;
        vResDesc.SampleDesc.Count = 1;
        vResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        vResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &vHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &vResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_vertexBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_vertexBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, vertices.data(), sizeof(VertexTexCoord2D) * 4);
            s_vertexBuffer->Unmap(0, nullptr);
        }
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(2);
        indices.emplace_back(1);
        indices.emplace_back(3);

        D3D12_HEAP_PROPERTIES ibHeapProps = {};
        ibHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        ibHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        ibHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC ibResDesc = {};
        ibResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibResDesc.Width = sizeof(uint32_t) * 6;
        ibResDesc.Height = 1;
        ibResDesc.DepthOrArraySize = 1;
        ibResDesc.MipLevels = 1;
        ibResDesc.Format = DXGI_FORMAT_UNKNOWN;
        ibResDesc.SampleDesc.Count = 1;
        ibResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        ibResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &ibHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &ibResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_indexBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_indexBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, indices.data(), sizeof(uint32_t) * 6);
            s_indexBuffer->Unmap(0, nullptr);
        }
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.DepthClipEnable = true;
        // depth
        psoDesc.DepthStencilState.DepthEnable = false;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // blend
        psoDesc.BlendState.AlphaToCoverageEnable = false;
        psoDesc.BlendState.IndependentBlendEnable = false;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
        rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        rtBlendDesc.BlendEnable = true;
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
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;
        // root signature
        std::vector<D3D12_DESCRIPTOR_RANGE> descTableRange;
        descTableRange.push_back({});
        descTableRange.at(0).NumDescriptors = 1;
        descTableRange.at(0).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(0).BaseShaderRegister = 0;
        descTableRange.at(0).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        std::vector<D3D12_ROOT_PARAMETER> rootParam;
        rootParam.push_back({});
        rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(0).DescriptorTable.pDescriptorRanges = &descTableRange.at(0);
        rootParam.at(0).DescriptorTable.NumDescriptorRanges = 1;

        D3D12_STATIC_SAMPLER_DESC samplerDescs[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_STATIC_SAMPLER_DESC& samplerDesc = samplerDescs[i];
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.ShaderRegister = i;
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        }
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.pParameters = rootParam.data();
        rootSignatureDesc.NumParameters = rootParam.size();
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.pStaticSamplers = samplerDescs;
        rootSignatureDesc.NumStaticSamplers = 3;
        ComPtr<ID3DBlob> rootSigBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;
        if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob))) {
            throw std::runtime_error("failed D3D12SerializeRootSignature()");
        }
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&s_rootSignature)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = s_rootSignature.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&s_pipelineState)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
        //
        // Descriptor Heap
        //
        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 1;
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&s_descriptorHeap)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }

        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        device->CreateShaderResourceView(bloomTextures.at(0).Get(), &srvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    // 1番をソースとして2番にブラー後の結果を書き込む
    {

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        // input layout
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
        psoDesc.InputLayout.NumElements = inputLayout.size();
        // shader
        std::unordered_map<std::string, std::string> shaderKeywords;
        s_shader2 = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {
            Output output;
            output.svpos = float4(pos, 0, 1);
            output.texCoord = texCoord;
            return output;
        })"),
                                        shaderKeywords),
            "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Texture2D<float4> srcTex : register(t0);
        SamplerState srcSmp : register(s0);

        float4 psMain(Output input) : SV_TARGET {            
            float4 result = 0;
            float offset[5] = { -2.0, -1.0, 0.0, 1.0, 2.0 };
            float weight[5] = { 0.1, 0.2, 0.4, 0.2, 0.1 };
            float2 texelSize = float2(1.0 / 800.0, 1.0 / 600.0);

            for (int i = 0; i < 5; i++)
            {
                float2 samplePos = input.texCoord + float2(0, offset[i]) * texelSize.y;
                result += srcTex.Sample(srcSmp, samplePos) * weight[i];
            }
            return result;
        })",
            "psMain");
        s_shader2->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
        // vertex buffer and index buffer
        std::vector<VertexTexCoord2D> vertices;
        std::vector<uint32_t> indices;
        const float half = 1.0f;
        const float left = -half;
        const float right = half;
        const float top = half;
        const float bottom = -half;
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));

        D3D12_HEAP_PROPERTIES vHeapProps = {};
        vHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        vHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC vResDesc = {};
        vResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vResDesc.Width = sizeof(VertexTexCoord2D) * 4;
        vResDesc.Height = 1;
        vResDesc.DepthOrArraySize = 1;
        vResDesc.MipLevels = 1;
        vResDesc.Format = DXGI_FORMAT_UNKNOWN;
        vResDesc.SampleDesc.Count = 1;
        vResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        vResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &vHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &vResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_vertexBuffer2)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_vertexBuffer2->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, vertices.data(), sizeof(VertexTexCoord2D) * 4);
            s_vertexBuffer2->Unmap(0, nullptr);
        }
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(2);
        indices.emplace_back(1);
        indices.emplace_back(3);

        D3D12_HEAP_PROPERTIES ibHeapProps = {};
        ibHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        ibHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        ibHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC ibResDesc = {};
        ibResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibResDesc.Width = sizeof(uint32_t) * 6;
        ibResDesc.Height = 1;
        ibResDesc.DepthOrArraySize = 1;
        ibResDesc.MipLevels = 1;
        ibResDesc.Format = DXGI_FORMAT_UNKNOWN;
        ibResDesc.SampleDesc.Count = 1;
        ibResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        ibResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &ibHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &ibResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_indexBuffer2)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_indexBuffer2->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, indices.data(), sizeof(uint32_t) * 6);
            s_indexBuffer2->Unmap(0, nullptr);
        }
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.DepthClipEnable = true;
        // depth
        psoDesc.DepthStencilState.DepthEnable = false;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // blend
        psoDesc.BlendState.AlphaToCoverageEnable = false;
        psoDesc.BlendState.IndependentBlendEnable = false;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
        rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        rtBlendDesc.BlendEnable = true;
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
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;
        // root signature
        std::vector<D3D12_DESCRIPTOR_RANGE> descTableRange;
        descTableRange.push_back({});
        descTableRange.at(0).NumDescriptors = 1;
        descTableRange.at(0).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(0).BaseShaderRegister = 0;
        descTableRange.at(0).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        std::vector<D3D12_ROOT_PARAMETER> rootParam;
        rootParam.push_back({});
        rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(0).DescriptorTable.pDescriptorRanges = &descTableRange.at(0);
        rootParam.at(0).DescriptorTable.NumDescriptorRanges = 1;

        D3D12_STATIC_SAMPLER_DESC samplerDescs[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_STATIC_SAMPLER_DESC& samplerDesc = samplerDescs[i];
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.ShaderRegister = i;
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        }
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.pParameters = rootParam.data();
        rootSignatureDesc.NumParameters = rootParam.size();
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.pStaticSamplers = samplerDescs;
        rootSignatureDesc.NumStaticSamplers = 3;
        ComPtr<ID3DBlob> rootSigBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;
        if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob))) {
            throw std::runtime_error("failed D3D12SerializeRootSignature()");
        }
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&s_rootSignature2)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = s_rootSignature2.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&s_pipelineState2)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
        //
        // Descriptor Heap
        //
        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 1;
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&s_descriptorHeap2)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }

        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap2->GetCPUDescriptorHandleForHeapStart();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        device->CreateShaderResourceView(bloomTextures.at(1).Get(), &srvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    // 2番をソースとして1番にブラー後
    {

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        // input layout
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
        psoDesc.InputLayout.NumElements = inputLayout.size();
        // shader
        std::unordered_map<std::string, std::string> shaderKeywords;
        s_shader3 = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {
            Output output;
            output.svpos = float4(pos, 0, 1);
            output.texCoord = texCoord;
            return output;
        })"),
                                        shaderKeywords),
            "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Texture2D<float4> srcTex : register(t0);
        SamplerState srcSmp : register(s0);

        float4 psMain(Output input) : SV_TARGET {            
            float4 result = 0;
            float offset[5] = { -2.0, -1.0, 0.0, 1.0, 2.0 };
            float weight[5] = { 0.1, 0.2, 0.4, 0.2, 0.1 }; // ガウス分布の重み
            float2 texelSize = float2(1.0 / 800.0, 1.0 / 600.0);

            for (int i = 0; i < 5; i++)
            {
                float2 samplePos = input.texCoord + float2(offset[i], 0) * texelSize.x;
                result += srcTex.Sample(srcSmp, samplePos) * weight[i];
            }
            return result;
        })",
            "psMain");
        s_shader3->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
        // vertex buffer and index buffer
        std::vector<VertexTexCoord2D> vertices;
        std::vector<uint32_t> indices;
        const float half = 1.0f;
        const float left = -half;
        const float right = half;
        const float top = half;
        const float bottom = -half;
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));

        D3D12_HEAP_PROPERTIES vHeapProps = {};
        vHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        vHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC vResDesc = {};
        vResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vResDesc.Width = sizeof(VertexTexCoord2D) * 4;
        vResDesc.Height = 1;
        vResDesc.DepthOrArraySize = 1;
        vResDesc.MipLevels = 1;
        vResDesc.Format = DXGI_FORMAT_UNKNOWN;
        vResDesc.SampleDesc.Count = 1;
        vResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        vResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &vHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &vResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_vertexBuffer3)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_vertexBuffer3->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, vertices.data(), sizeof(VertexTexCoord2D) * 4);
            s_vertexBuffer3->Unmap(0, nullptr);
        }
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(2);
        indices.emplace_back(1);
        indices.emplace_back(3);

        D3D12_HEAP_PROPERTIES ibHeapProps = {};
        ibHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        ibHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        ibHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC ibResDesc = {};
        ibResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibResDesc.Width = sizeof(uint32_t) * 6;
        ibResDesc.Height = 1;
        ibResDesc.DepthOrArraySize = 1;
        ibResDesc.MipLevels = 1;
        ibResDesc.Format = DXGI_FORMAT_UNKNOWN;
        ibResDesc.SampleDesc.Count = 1;
        ibResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        ibResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &ibHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &ibResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_indexBuffer3)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_indexBuffer3->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, indices.data(), sizeof(uint32_t) * 6);
            s_indexBuffer3->Unmap(0, nullptr);
        }
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.DepthClipEnable = true;
        // depth
        psoDesc.DepthStencilState.DepthEnable = false;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // blend
        psoDesc.BlendState.AlphaToCoverageEnable = false;
        psoDesc.BlendState.IndependentBlendEnable = false;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
        rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        rtBlendDesc.BlendEnable = true;
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
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;
        // root signature
        std::vector<D3D12_DESCRIPTOR_RANGE> descTableRange;
        descTableRange.push_back({});
        descTableRange.at(0).NumDescriptors = 1;
        descTableRange.at(0).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(0).BaseShaderRegister = 0;
        descTableRange.at(0).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        std::vector<D3D12_ROOT_PARAMETER> rootParam;
        rootParam.push_back({});
        rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(0).DescriptorTable.pDescriptorRanges = &descTableRange.at(0);
        rootParam.at(0).DescriptorTable.NumDescriptorRanges = 1;

        D3D12_STATIC_SAMPLER_DESC samplerDescs[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_STATIC_SAMPLER_DESC& samplerDesc = samplerDescs[i];
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.ShaderRegister = i;
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        }
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.pParameters = rootParam.data();
        rootSignatureDesc.NumParameters = rootParam.size();
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.pStaticSamplers = samplerDescs;
        rootSignatureDesc.NumStaticSamplers = 3;
        ComPtr<ID3DBlob> rootSigBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;
        if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob))) {
            throw std::runtime_error("failed D3D12SerializeRootSignature()");
        }
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&s_rootSignature3)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = s_rootSignature3.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&s_pipelineState3)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
        //
        // Descriptor Heap
        //
        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 1;
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&s_descriptorHeap3)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }

        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap3->GetCPUDescriptorHandleForHeapStart();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        device->CreateShaderResourceView(bloomTextures.at(2).Get(), &srvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
    // 0,1番をソースとしてダイレクトに書き込む
    {

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        // input layout
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
        inputLayout.push_back(
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        inputLayout.push_back(
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                D3D12_APPEND_ALIGNED_ELEMENT,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
        psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
        psoDesc.InputLayout.NumElements = inputLayout.size();
        // shader
        std::unordered_map<std::string, std::string> shaderKeywords;
        s_shader4 = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Output vsMain(float2 pos : POSITION, float2 texCoord : TEXCOORD) {
            Output output;
            output.svpos = float4(pos, 0, 1);
            output.texCoord = texCoord;
            return output;
        })"),
                                        shaderKeywords),
            "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
            float2 texCoord : TEXCOORD;
        };

        Texture2D<float4> srcTex : register(t0);
        SamplerState srcSmp : register(s0);

        Texture2D<float4> blurTex : register(t1);
        SamplerState blurSmp : register(s1);

        float4 psMain(Output input) : SV_TARGET {      
            float4 sceneColor = srcTex.Sample(srcSmp, input.texCoord);
            float4 bloomColor = blurTex.Sample(blurSmp, input.texCoord) * 1.0;
            float4 col =  sceneColor + bloomColor;
            col = float4(col.rgb / (1.0 + col.rgb), 1.0);
            return col;
        })",
            "psMain");
        s_shader4->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
        // vertex buffer and index buffer
        std::vector<VertexTexCoord2D> vertices;
        std::vector<uint32_t> indices;
        const float half = 1.0f;
        const float left = -half;
        const float right = half;
        const float top = half;
        const float bottom = -half;
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
        vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));

        D3D12_HEAP_PROPERTIES vHeapProps = {};
        vHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        vHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC vResDesc = {};
        vResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vResDesc.Width = sizeof(VertexTexCoord2D) * 4;
        vResDesc.Height = 1;
        vResDesc.DepthOrArraySize = 1;
        vResDesc.MipLevels = 1;
        vResDesc.Format = DXGI_FORMAT_UNKNOWN;
        vResDesc.SampleDesc.Count = 1;
        vResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        vResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &vHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &vResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_vertexBuffer4)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_vertexBuffer4->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, vertices.data(), sizeof(VertexTexCoord2D) * 4);
            s_vertexBuffer4->Unmap(0, nullptr);
        }
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(2);
        indices.emplace_back(1);
        indices.emplace_back(3);

        D3D12_HEAP_PROPERTIES ibHeapProps = {};
        ibHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        ibHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        ibHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC ibResDesc = {};
        ibResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibResDesc.Width = sizeof(uint32_t) * 6;
        ibResDesc.Height = 1;
        ibResDesc.DepthOrArraySize = 1;
        ibResDesc.MipLevels = 1;
        ibResDesc.Format = DXGI_FORMAT_UNKNOWN;
        ibResDesc.SampleDesc.Count = 1;
        ibResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        ibResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &ibHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &ibResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&s_indexBuffer4)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(s_indexBuffer4->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, indices.data(), sizeof(uint32_t) * 6);
            s_indexBuffer4->Unmap(0, nullptr);
        }
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.DepthClipEnable = true;
        // depth
        psoDesc.DepthStencilState.DepthEnable = false;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // blend
        psoDesc.BlendState.AlphaToCoverageEnable = false;
        psoDesc.BlendState.IndependentBlendEnable = false;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
        rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        rtBlendDesc.BlendEnable = true;
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
        descTableRange.at(0).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(0).BaseShaderRegister = 0;
        descTableRange.at(0).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        descTableRange.push_back({});
        descTableRange.at(1).NumDescriptors = 1;
        descTableRange.at(1).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(1).BaseShaderRegister = 1;
        descTableRange.at(1).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        std::vector<D3D12_ROOT_PARAMETER> rootParam;
        rootParam.push_back({});
        rootParam.at(0).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(0).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(0).DescriptorTable.pDescriptorRanges = &descTableRange.at(0);
        rootParam.at(0).DescriptorTable.NumDescriptorRanges = 1;
        rootParam.push_back({});
        rootParam.at(1).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(1).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(1).DescriptorTable.pDescriptorRanges = &descTableRange.at(1);
        rootParam.at(1).DescriptorTable.NumDescriptorRanges = 1;

        D3D12_STATIC_SAMPLER_DESC samplerDescs[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_STATIC_SAMPLER_DESC& samplerDesc = samplerDescs[i];
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.ShaderRegister = i;
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        }
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.pParameters = rootParam.data();
        rootSignatureDesc.NumParameters = rootParam.size();
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        rootSignatureDesc.pStaticSamplers = samplerDescs;
        rootSignatureDesc.NumStaticSamplers = 3;
        ComPtr<ID3DBlob> rootSigBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;
        if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob))) {
            throw std::runtime_error("failed D3D12SerializeRootSignature()");
        }
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&s_rootSignature4)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = s_rootSignature4.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&s_pipelineState4)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
        //
        // Descriptor Heap
        //
        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 2;
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&s_descriptorHeap4)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }

        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = s_descriptorHeap4->GetCPUDescriptorHandleForHeapStart();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        device->CreateShaderResourceView(bloomTextures.at(0).Get(), &srvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        device->CreateShaderResourceView(bloomTextures.at(1).Get(), &srvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
}
void BloomEffect::destroy()
{
}
// private
}