#include <Graphics/Buffer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/PointLight.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Math/Matrix.hpp>
#include <Utils/String.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// internal
void PointLight::draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
    {

        {
            void* outData;
            if (FAILED(m_modelMatrixBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            float scale = 20.0f / 10.0f;
            Math::Matrix mat = Math::Matrix::translate(Math::Vector3({ 8 * 5, 10, 8 * 5 }));
            mat = Math::Matrix::scale(Math::Vector3({ scale, scale, scale })) * mat;
            ::memcpy(outData, mat.data(), sizeof(Math::Matrix));
            m_modelMatrixBuffer->Unmap(0, nullptr);
        }
        {
            void* outData;
            if (FAILED(m_viewMatrixBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            Math::Matrix mat = Camera::getLookAtMatrix();
            ::memcpy(outData, mat.data(), sizeof(Math::Matrix));
            m_viewMatrixBuffer->Unmap(0, nullptr);
        }
        {
            void* outData;
            if (FAILED(m_projectionMatrixBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            Math::Matrix mat = Camera::getPerspectiveMatrix();
            ::memcpy(outData, mat.data(), sizeof(Math::Matrix));
            m_projectionMatrixBuffer->Unmap(0, nullptr);
        }
        commandList->SetPipelineState(m_pipelineState.Get());
        commandList->SetGraphicsRootSignature(m_rootSignature.Get());

        auto device = Engine::getInstance()->getDevice()->getID3D12Device();
        uint64_t incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
        commandList->SetDescriptorHeaps(1, m_descriptorHeap.GetAddressOf());

        for (int32_t i = 0; i < 6; i++) {
            commandList->SetGraphicsRootDescriptorTable(i, heapHandle);
            heapHandle.ptr += incrementSize;
        }
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D12_VERTEX_BUFFER_VIEW vbView = {};
        vbView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        vbView.SizeInBytes = sizeof(Math::Vector3) * m_vertexLength;
        vbView.StrideInBytes = static_cast<UINT>(sizeof(Math::Vector3));
        commandList->IASetVertexBuffers(0, 1, &vbView);

        D3D12_INDEX_BUFFER_VIEW ibView = {};
        ibView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R32_UINT;
        ibView.SizeInBytes = sizeof(uint32_t) * m_indexLength;
        commandList->IASetIndexBuffer(&ibView);

        commandList->DrawIndexedInstanced(m_indexLength, 1, 0, 0, 0);
    }

    //
    // Stencil
    //

    {

        commandList->SetPipelineState(m_scrPipelineState.Get());
        commandList->SetGraphicsRootSignature(m_scrRootSignature.Get());

        auto device = Engine::getInstance()->getDevice()->getID3D12Device();
        uint64_t incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        D3D12_GPU_DESCRIPTOR_HANDLE heapHandle = m_scrDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        commandList->SetDescriptorHeaps(1, m_scrDescriptorHeap.GetAddressOf());

        for (int32_t i = 0; i < 3; i++) {
            commandList->SetGraphicsRootDescriptorTable(i, heapHandle);
            heapHandle.ptr += incrementSize;
        }
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D12_VERTEX_BUFFER_VIEW vbView = {};
        vbView.BufferLocation = m_scrVertexBuffer->GetGPUVirtualAddress();
        vbView.SizeInBytes = sizeof(VertexTexCoord2D) * 4;
        vbView.StrideInBytes = static_cast<UINT>(sizeof(VertexTexCoord2D));
        commandList->IASetVertexBuffers(0, 1, &vbView);

        D3D12_INDEX_BUFFER_VIEW ibView = {};
        ibView.BufferLocation = m_scrIndexBuffer->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R32_UINT;
        ibView.SizeInBytes = sizeof(uint32_t) * 6;
        commandList->IASetIndexBuffer(&ibView);

        commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
    }
}

std::shared_ptr<PointLight> PointLight::create(
    const Microsoft::WRL::ComPtr<ID3D12Device>& device,
    const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& gTextures)
{
    auto globalLight = std::shared_ptr<PointLight>(new PointLight());
    {

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        // input layout
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
        std::unordered_map<std::string, std::string> shaderKeywords;
        globalLight->m_shader = Shader::compile(Utils::String::interpolate(std::string(R"(
        struct Output {
            float4 svpos : SV_POSITION;
        };
        cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
        cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
        cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }

        Output vsMain(float3 pos : POSITION) {
            Output output;
            output.svpos = mul(modelMatrix, float4(pos, 1));
            output.svpos = mul(viewMatrix, output.svpos);
            output.svpos = mul(projectionMatrix, output.svpos);
            return output;
        })"),
                                                    shaderKeywords),
            "vsMain", R"(
        struct Output {
            float4 svpos : SV_POSITION;
        };

        Texture2D<float4> positionTex : register(t0);
        SamplerState positionSmp : register(s0);

        Texture2D<float4> normalTex : register(t1);
        SamplerState normalSmp : register(s1);

        Texture2D<float4> colorTex : register(t2);
        SamplerState colorSmp : register(s2);

        float4 psMain(Output input) : SV_TARGET {
            float svx = input.svpos.x / 800.0;
            float svy = input.svpos.y / 600.0;
            float2 coord = float2(
                svx, // ((svx + 1.0) * 0.5),
                svy // 1.0 - ((svy + 1.0) * 0.5)
            );
            float4 positionCol = positionTex.Sample(positionSmp, coord);
            float4 normalCol = normalTex.Sample(normalSmp, coord);
            float4 colorCol = colorTex.Sample(colorSmp, coord);
            float3 uPointLightPos = float3(8 * 5, 10, 8 * 5);

            float3 N = normalize(normalCol.xyz);
            float3 L = normalize(uPointLightPos - positionCol.xyz);
            float3 Phong = float3(0, 0, 0);
            float NdotL = dot(N, L);

            if (NdotL > 0)
            {
                // Get the distance between the light and the world pos
                float dist = distance(uPointLightPos, positionCol.xyz);
                // Use smoothstep to compute value in range [0,1]
                // between inner/outer radius
                float intensity = smoothstep(5/* uPointLight.mInnerRadius */,
                                            20/* uPointLight.mOuterRadius */, dist);
                // The diffuse color of the light depends on intensity
                float3 DiffuseColor = lerp(float3(1, 1, 1),
                                        float3(0.0, 0.0, 0.0), intensity);
                Phong = DiffuseColor * NdotL;
            }
            // return float4(input.svpos.xy, 0, 1);
            // return float4(coord, 0, 1);
            // return float4(colorCol.xyz, 1);
            // return float4(colorCol.xyz * Phong, 1.0);
            return float4(0, 0, 0, 1);
        })",
            "psMain");
        globalLight->m_shader->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
        // vertex buffer and index buffer
        std::vector<Math::Vector3> vertices;
        std::vector<uint32_t> indices;
        generateSphere(10, 10, 10, vertices, indices);
        globalLight->m_vertexLength = static_cast<int32_t>(vertices.size());
        globalLight->m_indexLength = static_cast<int32_t>(indices.size());
        // const float half = 1.0f;
        // const float left = -half;
        // const float right = half;
        // const float top = half;
        // const float bottom = -half;
        // vertices.push_back(Math::Vector3(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));
        // vertices.push_back(Math::Vector3(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
        // vertices.push_back(Math::Vector3(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
        // vertices.push_back(Math::Vector3(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));

        D3D12_HEAP_PROPERTIES vHeapProps = {};
        vHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        vHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        vHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC vResDesc = {};
        vResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vResDesc.Width = sizeof(Math::Vector3) * globalLight->m_vertexLength;
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
                IID_PPV_ARGS(&globalLight->m_vertexBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(globalLight->m_vertexBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, vertices.data(), sizeof(Math::Vector3) * globalLight->m_vertexLength);
            globalLight->m_vertexBuffer->Unmap(0, nullptr);
        }
        // globalLight->m_vertexBuffer = Buffer::create();
        // globalLight->m_vertexBuffer->allocate(sizeof(Math::Vector3) * vertices.size());
        // globalLight->m_vertexBuffer->update(vertices.data());

        D3D12_HEAP_PROPERTIES ibHeapProps = {};
        ibHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        ibHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        ibHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC ibResDesc = {};
        ibResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibResDesc.Width = sizeof(uint32_t) * globalLight->m_indexLength;
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
                IID_PPV_ARGS(&globalLight->m_indexBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(globalLight->m_indexBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, indices.data(), sizeof(uint32_t) * globalLight->m_indexLength);
            globalLight->m_indexBuffer->Unmap(0, nullptr);
        }
        // globalLight->m_indexBuffer = Buffer::create();
        // globalLight->m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
        // globalLight->m_indexBuffer->update(indices.data());
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.DepthClipEnable = true;
        // depth
        psoDesc.DepthStencilState.DepthEnable = true;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // stencil
        psoDesc.DepthStencilState.StencilEnable = true;
        psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        // stencil front
        psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        // stencil back
        psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR_SAT;
        psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        // blend
        psoDesc.BlendState.AlphaToCoverageEnable = false;
        psoDesc.BlendState.IndependentBlendEnable = false;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
        rtBlendDesc.BlendEnable = false;
        rtBlendDesc.LogicOpEnable = false;
        rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        rtBlendDesc.BlendEnable = true;
        rtBlendDesc.LogicOpEnable = false;
        rtBlendDesc.SrcBlend = D3D12_BLEND_ONE;
        rtBlendDesc.DestBlend = D3D12_BLEND_ONE;
        rtBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
        rtBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
        rtBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
        rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        rtBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
        psoDesc.BlendState.RenderTarget[0] = rtBlendDesc;
        psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
        descTableRange.push_back({});
        descTableRange.at(2).NumDescriptors = 1;
        descTableRange.at(2).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(2).BaseShaderRegister = 2;
        descTableRange.at(2).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        descTableRange.push_back({});
        descTableRange.at(3).NumDescriptors = 1;
        descTableRange.at(3).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(3).BaseShaderRegister = 0;
        descTableRange.at(3).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        descTableRange.push_back({});
        descTableRange.at(4).NumDescriptors = 1;
        descTableRange.at(4).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(4).BaseShaderRegister = 1;
        descTableRange.at(4).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        descTableRange.push_back({});
        descTableRange.at(5).NumDescriptors = 1;
        descTableRange.at(5).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descTableRange.at(5).BaseShaderRegister = 2;
        descTableRange.at(5).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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
        rootParam.push_back({});
        rootParam.at(2).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(2).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(2).DescriptorTable.pDescriptorRanges = &descTableRange.at(2);
        rootParam.at(2).DescriptorTable.NumDescriptorRanges = 1;
        rootParam.push_back({});
        rootParam.at(3).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(3).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        rootParam.at(3).DescriptorTable.pDescriptorRanges = &descTableRange.at(3);
        rootParam.at(3).DescriptorTable.NumDescriptorRanges = 1;
        rootParam.push_back({});
        rootParam.at(4).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(4).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        rootParam.at(4).DescriptorTable.pDescriptorRanges = &descTableRange.at(4);
        rootParam.at(4).DescriptorTable.NumDescriptorRanges = 1;
        rootParam.push_back({});
        rootParam.at(5).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(5).ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        rootParam.at(5).DescriptorTable.pDescriptorRanges = &descTableRange.at(5);
        rootParam.at(5).DescriptorTable.NumDescriptorRanges = 1;

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
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&globalLight->m_rootSignature)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = globalLight->m_rootSignature.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&globalLight->m_pipelineState)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
        //
        // Descriptor Heap
        //

        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 6;
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&globalLight->m_descriptorHeap)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }

        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = globalLight->m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        for (int32_t i = 0; i < static_cast<int32_t>(gTextures.size()); i++) {
            device->CreateShaderResourceView(gTextures.at(i).Get(), &srvDesc, heapHandle);
            heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        //
        // ModelMatrix
        //

        D3D12_HEAP_PROPERTIES modelMatHeapProps = {};
        modelMatHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        modelMatHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        modelMatHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC modelMatResDesc = {};
        modelMatResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        modelMatResDesc.Width = 256;
        modelMatResDesc.Height = 1;
        modelMatResDesc.DepthOrArraySize = 1;
        modelMatResDesc.MipLevels = 1;
        modelMatResDesc.Format = DXGI_FORMAT_UNKNOWN;
        modelMatResDesc.SampleDesc.Count = 1;
        modelMatResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        modelMatResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &modelMatHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &modelMatResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&globalLight->m_modelMatrixBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        D3D12_CONSTANT_BUFFER_VIEW_DESC modelCbvDesc = {};
        modelCbvDesc.BufferLocation = globalLight->m_modelMatrixBuffer->GetGPUVirtualAddress();
        modelCbvDesc.SizeInBytes = 256;

        device->CreateConstantBufferView(&modelCbvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        //
        // ViewMatrix
        //

        D3D12_HEAP_PROPERTIES viewMatHeapProps = {};
        viewMatHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        viewMatHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        viewMatHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC viewMatResDesc = {};
        viewMatResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        viewMatResDesc.Width = 256;
        viewMatResDesc.Height = 1;
        viewMatResDesc.DepthOrArraySize = 1;
        viewMatResDesc.MipLevels = 1;
        viewMatResDesc.Format = DXGI_FORMAT_UNKNOWN;
        viewMatResDesc.SampleDesc.Count = 1;
        viewMatResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        viewMatResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &viewMatHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &viewMatResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&globalLight->m_viewMatrixBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        D3D12_CONSTANT_BUFFER_VIEW_DESC viewCbvDesc = {};
        viewCbvDesc.BufferLocation = globalLight->m_viewMatrixBuffer->GetGPUVirtualAddress();
        viewCbvDesc.SizeInBytes = 256;

        device->CreateConstantBufferView(&viewCbvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        //
        // ProjectionMatrix
        //

        D3D12_HEAP_PROPERTIES projMatHeapProps = {};
        projMatHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        projMatHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        projMatHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC projMatResDesc = {};
        projMatResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        projMatResDesc.Width = 256;
        projMatResDesc.Height = 1;
        projMatResDesc.DepthOrArraySize = 1;
        projMatResDesc.MipLevels = 1;
        projMatResDesc.Format = DXGI_FORMAT_UNKNOWN;
        projMatResDesc.SampleDesc.Count = 1;
        projMatResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        projMatResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        if (FAILED(device->CreateCommittedResource(
                &projMatHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &projMatResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&globalLight->m_projectionMatrixBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        D3D12_CONSTANT_BUFFER_VIEW_DESC projCbvDesc = {};
        projCbvDesc.BufferLocation = globalLight->m_projectionMatrixBuffer->GetGPUVirtualAddress();
        projCbvDesc.SizeInBytes = 256;
        device->CreateConstantBufferView(&projCbvDesc, heapHandle);
        heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }
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
        globalLight->m_scrShader = Shader::compile(Utils::String::interpolate(std::string(R"(
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

        Texture2D<float4> positionTex : register(t0);
        SamplerState positionSmp : register(s0);

        Texture2D<float4> normalTex : register(t1);
        SamplerState normalSmp : register(s1);

        Texture2D<float4> colorTex : register(t2);
        SamplerState colorSmp : register(s2);

        float4 psMain(Output input) : SV_TARGET {
            float svx = input.texCoord.x;
            float svy = input.texCoord.y;
            float2 coord = float2(
                svx, // ((svx + 1.0) * 0.5),
                svy // 1.0 - ((svy + 1.0) * 0.5)
            );
            float4 positionCol = positionTex.Sample(positionSmp, coord);
            float4 normalCol = normalTex.Sample(normalSmp, coord);
            float4 colorCol = colorTex.Sample(colorSmp, coord);
            float3 uPointLightPos = float3(8 * 5, 10, 8 * 5);

            float3 N = normalize(normalCol.xyz);
            float3 L = normalize(uPointLightPos - positionCol.xyz);
            float3 Phong = float3(0, 0, 0);
            float NdotL = (dot(N, L) + 1) * 0.5;

            if (NdotL > 0)
            {
                // Get the distance between the light and the world pos
                float dist = distance(uPointLightPos, positionCol.xyz);
                // Use smoothstep to compute value in range [0,1]
                // between inner/outer radius
                float intensity = smoothstep(5/* uPointLight.mInnerRadius */,
                                            20/* uPointLight.mOuterRadius */, dist);
                // The diffuse color of the light depends on intensity
                float3 DiffuseColor = lerp(float3(1, 1, 1),
                                        float3(0.0, 0.0, 0.0), intensity);
                Phong = DiffuseColor * NdotL;
            }
            // return float4(input.svpos.xy, 0, 1);
            // return float4(coord, 0, 1);
            // return float4(colorCol.xyz, 1);
            return float4(colorCol.xyz * Phong, 1.0);
        })",
            "psMain");
        globalLight->m_scrShader->getD3D12_SHADER_BYTECODE(psoDesc.VS, psoDesc.PS);
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
                IID_PPV_ARGS(&globalLight->m_scrVertexBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(globalLight->m_scrVertexBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, vertices.data(), sizeof(VertexTexCoord2D) * 4);
            globalLight->m_scrVertexBuffer->Unmap(0, nullptr);
        }
        // globalLight->m_vertexBuffer = Buffer::create();
        // globalLight->m_vertexBuffer->allocate(sizeof(VertexTexCoord2D) * vertices.size());
        // globalLight->m_vertexBuffer->update(vertices.data());
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
                IID_PPV_ARGS(&globalLight->m_scrIndexBuffer)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }
        {
            void* outData;
            if (FAILED(globalLight->m_scrIndexBuffer->Map(0, nullptr, (void**)&outData))) {
                throw std::runtime_error("failed Map()");
            }
            ::memcpy(outData, indices.data(), sizeof(uint32_t) * 6);
            globalLight->m_scrIndexBuffer->Unmap(0, nullptr);
        }
        // globalLight->m_indexBuffer = Buffer::create();
        // globalLight->m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
        // globalLight->m_indexBuffer->update(indices.data());
        // rasterize
        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.DepthClipEnable = true;
        // depth
        psoDesc.DepthStencilState.DepthEnable = false;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        // stencil
        psoDesc.DepthStencilState.StencilEnable = true;
        psoDesc.DepthStencilState.StencilWriteMask = 0;
        psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        // stencil front
        psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
        // stencil back
        psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
        // blend
        psoDesc.BlendState.AlphaToCoverageEnable = false;
        psoDesc.BlendState.IndependentBlendEnable = false;
        D3D12_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
        rtBlendDesc.BlendEnable = false;
        rtBlendDesc.LogicOpEnable = false;
        rtBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        rtBlendDesc.BlendEnable = true;
        rtBlendDesc.LogicOpEnable = false;
        rtBlendDesc.SrcBlend = D3D12_BLEND_ONE;
        rtBlendDesc.DestBlend = D3D12_BLEND_ONE;
        rtBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
        rtBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
        rtBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
        rtBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        rtBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
        psoDesc.BlendState.RenderTarget[0] = rtBlendDesc;
        psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
        descTableRange.push_back({});
        descTableRange.at(2).NumDescriptors = 1;
        descTableRange.at(2).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descTableRange.at(2).BaseShaderRegister = 2;
        descTableRange.at(2).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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
        rootParam.push_back({});
        rootParam.at(2).ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam.at(2).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        rootParam.at(2).DescriptorTable.pDescriptorRanges = &descTableRange.at(2);
        rootParam.at(2).DescriptorTable.NumDescriptorRanges = 1;

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
        if (FAILED(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&globalLight->m_scrRootSignature)))) {
            throw std::runtime_error("failed CreateRootSignature()");
        }
        psoDesc.pRootSignature = globalLight->m_scrRootSignature.Get();
        if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&globalLight->m_scrPipelineState)))) {
            throw std::runtime_error("failed CreateGraphicsPipelineState()");
        }
        //
        // Descriptor Heap
        //

        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 3;
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&globalLight->m_scrDescriptorHeap)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }

        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = globalLight->m_scrDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        for (int32_t i = 0; i < static_cast<int32_t>(gTextures.size()); i++) {
            device->CreateShaderResourceView(gTextures.at(i).Get(), &srvDesc, heapHandle);
            heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
    }
    return globalLight;
}

void PointLight::destroy()
{
}
// private
PointLight::PointLight()
    : m_shader()
    , m_scrShader()
    , m_vertexLength()
    , m_indexLength()
    , m_pipelineState()
    , m_rootSignature()
    , m_descriptorHeap()
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_modelMatrixBuffer()
    , m_viewMatrixBuffer()
    , m_projectionMatrixBuffer()
    , m_scrPipelineState()
    , m_scrRootSignature()
    , m_scrDescriptorHeap()
{
}
void PointLight::generateSphere(int32_t radius, int32_t latitudes, int32_t longitudes, std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices)
{
    const float M_PI = 3.14f;
    if (longitudes < 3)
        longitudes = 3;
    if (latitudes < 2)
        latitudes = 2;

    // std::vector<Math::Vector3> vertices;
    // std::vector<Math::Vector3> normals;
    // std::vector<Math::Vector2> uv;
    // std::vector<unsigned int> indices;

    // float nx, ny, nz, lengthInv = 1.0f / radius; // normal
    // Temporary vertex
    struct Vertex {
        float x, y, z, s, t; // Postion and Texcoords
    };

    float deltaLatitude = M_PI / latitudes;
    float deltaLongitude = 2 * M_PI / longitudes;
    float latitudeAngle;
    float longitudeAngle;

    // Compute all vertices first except normals
    for (int i = 0; i <= latitudes; ++i) {
        latitudeAngle = M_PI / 2 - i * deltaLatitude; /* Starting -pi/2 to pi/2 */
        float xy = radius * cosf(latitudeAngle); /* r * cos(phi) */
        float z = radius * sinf(latitudeAngle); /* r * sin(phi )*/

        /*
         * We add (latitudes + 1) vertices per longitude because of equator,
         * the North pole and South pole are not counted here, as they overlap.
         * The first and last vertices have same position and normal, but
         * different tex coords.
         */
        for (int j = 0; j <= longitudes; ++j) {
            longitudeAngle = j * deltaLongitude;

            Vertex vertex;
            vertex.x = xy * cosf(longitudeAngle); /* x = r * cos(phi) * cos(theta)  */
            vertex.y = xy * sinf(longitudeAngle); /* y = r * cos(phi) * sin(theta) */
            vertex.z = z; /* z = r * sin(phi) */
            vertex.s = (float)j / longitudes; /* s */
            vertex.t = (float)i / latitudes; /* t */
            vertices.push_back(Math::Vector3({ vertex.x, vertex.y, vertex.z }));
            // uv.push_back(Math::Vector2({ vertex.s, vertex.t }));

            // normalized vertex normal
            // nx = vertex.x * lengthInv;
            // ny = vertex.y * lengthInv;
            // nz = vertex.z * lengthInv;
            // normals.push_back(Math::Vector3(nx, ny, nz));
        }
    }

    /*
     *  Indices
     *  k1--k1+1
     *  |  / |
     *  | /  |
     *  k2--k2+1
     */
    unsigned int k1, k2;
    for (int i = 0; i < latitudes; ++i) {
        k1 = i * (longitudes + 1);
        k2 = k1 + longitudes + 1;
        // 2 Triangles per latitude block excluding the first and last longitudes blocks
        for (int j = 0; j < longitudes; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (latitudes - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}
}