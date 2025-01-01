#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/GpuBuffer.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/UniformBuffer.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
void UniformBuffer::setVS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).vsUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    void* outData;
    ComPtr<ID3D12Resource> resource = m_vsResources.at(index);
    resource->Map(0, nullptr, (void**)&outData);
    ::memcpy(outData, data, Metadata::k_programs.at(m_entry).vsUniforms.at(index).size);
    resource->Unmap(0, nullptr);
}

void UniformBuffer::setGS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).gsUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    void* outData;
    ComPtr<ID3D12Resource> resource = m_gsResources.at(index);
    resource->Map(0, nullptr, (void**)&outData);
    ::memcpy(outData, data, Metadata::k_programs.at(m_entry).gsUniforms.at(index).size);
    resource->Unmap(0, nullptr);
}

void UniformBuffer::setPS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).psUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    Metadata::Uniform u = Metadata::k_programs.at(m_entry).psUniforms.at(index);
    bool isShaderResource = u.type == Metadata::Uniform::Type::SRV;
    if (isShaderResource) {
        throw std::logic_error("uniform is require texture.");
    } else {
        void* outData;
        ComPtr<ID3D12Resource> resource = m_psResources.at(index);
        resource->Map(0, nullptr, (void**)&outData);
        ::memcpy(outData, data, u.size);
        resource->Unmap(0, nullptr);
    }
}

void UniformBuffer::setPS(int32_t index, const std::shared_ptr<Texture>& texture)
{
    if (index >= Metadata::k_programs.at(m_entry).psUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    Metadata::Uniform u = Metadata::k_programs.at(m_entry).psUniforms.at(index);
    bool isShaderResource = u.type == Metadata::Uniform::Type::SRV;
    if (isShaderResource) {
        auto device = Engine::getInstance()->getDevice()->getID3D12Device();
        uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        heapHandle.ptr += Metadata::k_programs.at(m_entry).vsUniforms.size() * unitSize;
        heapHandle.ptr += Metadata::k_programs.at(m_entry).gsUniforms.size() * unitSize;
        heapHandle.ptr += index * unitSize;

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = Texture::encodeFormat(texture->getFormat());
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        auto texBuff = std::any_cast<ComPtr<ID3D12Resource>>(texture->getID3D12Resource());
        device->CreateShaderResourceView(texBuff.Get(), &srvDesc, heapHandle);
    } else {
        throw std::logic_error("uniform is require constant.");
    }
}

void UniformBuffer::setCS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).csUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    void* outData;
    ComPtr<ID3D12Resource> resource = m_csResources.at(index);
    resource->Map(0, nullptr, (void**)&outData);
    ::memcpy(outData, data, Metadata::k_programs.at(m_entry).csUniforms.at(index).size);
    resource->Unmap(0, nullptr);
}

void UniformBuffer::setCS(int32_t index, const std::shared_ptr<Texture>& texture)
{
    if (index >= Metadata::k_programs.at(m_entry).csUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    Metadata::Uniform u = Metadata::k_programs.at(m_entry).csUniforms.at(index);
    bool isShaderResource = u.type == Metadata::Uniform::Type::SRV;
    if (isShaderResource) {
        auto device = Engine::getInstance()->getDevice()->getID3D12Device();
        uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        heapHandle.ptr += Metadata::k_programs.at(m_entry).vsUniforms.size() * unitSize;
        heapHandle.ptr += Metadata::k_programs.at(m_entry).gsUniforms.size() * unitSize;
        heapHandle.ptr += Metadata::k_programs.at(m_entry).psUniforms.size() * unitSize;
        heapHandle.ptr += index * unitSize;

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = Texture::encodeFormat(texture->getFormat());
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        auto texBuff = std::any_cast<ComPtr<ID3D12Resource>>(texture->getID3D12Resource());
        device->CreateShaderResourceView(texBuff.Get(), &srvDesc, heapHandle);
    } else {
        throw std::logic_error("uniform is require constant.");
    }
}

void UniformBuffer::setCS(int32_t index, const std::shared_ptr<GpuBuffer>& buffer)
{
    if (index >= Metadata::k_programs.at(m_entry).csUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    Metadata::Uniform u = Metadata::k_programs.at(m_entry).csUniforms.at(index);
    bool isUnorderdAccessView = u.type == Metadata::Uniform::Type::UAV;
    if (isUnorderdAccessView) {
        if (index < m_uavBuffers.size() && m_uavBuffers.at(index) == buffer) {
            return;
        }
        auto device = Engine::getInstance()->getDevice()->getID3D12Device();
        uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        heapHandle.ptr += Metadata::k_programs.at(m_entry).vsUniforms.size() * unitSize;
        heapHandle.ptr += Metadata::k_programs.at(m_entry).gsUniforms.size() * unitSize;
        heapHandle.ptr += Metadata::k_programs.at(m_entry).psUniforms.size() * unitSize;
        heapHandle.ptr += index * unitSize;

        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = buffer->getSize() / u.size;
        uavDesc.Buffer.StructureByteStride = u.size;
        uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

        device->CreateUnorderedAccessView(buffer->getID3D12Resource().Get(), nullptr, &uavDesc, heapHandle);

        while (m_uavBuffers.size() < index + 1) {
            m_uavBuffers.emplace_back(nullptr);
        }
        m_uavBuffers.at(index) = buffer;
    } else {
        throw std::logic_error("uniform is require buffer.");
    }
}
// internal
std::shared_ptr<UniformBuffer> UniformBuffer::create(Metadata::ProgramTable entry)
{
    auto ub = std::shared_ptr<UniformBuffer>(new UniformBuffer());
    ub->init(entry);
    return ub;
}

void UniformBuffer::destroy()
{
}

void UniformBuffer::stateUAV(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    for (auto uavBuffer : m_uavBuffers) {
        if (uavBuffer) {
            uavBuffer->stateUAV(cmdList);
        }
    }
}
void UniformBuffer::stateCommon(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    for (auto uavBuffer : m_uavBuffers) {
        if (uavBuffer) {
            uavBuffer->stateCommon(cmdList);
        }
    }
}
void UniformBuffer::stateSync(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    for (auto uavBuffer : m_uavBuffers) {
        if (uavBuffer) {
            uavBuffer->stateSync(cmdList);
        }
    }
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UniformBuffer::getID3D12DescriptorHeap() const
{
    return m_descriptorHeap;
}
// private
UniformBuffer::UniformBuffer()
    : m_entry()
    , m_descriptorHeap()
    , m_vsResources()
    , m_gsResources()
    , m_psResources()
    , m_csResources()
    , m_uavBuffers()
{
}

void UniformBuffer::init(Metadata::ProgramTable entry)
{
    m_entry = entry;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    const Metadata::Program& program = Metadata::k_programs.at(entry);
    // descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NodeMask = 0;
    descHeapDesc.NumDescriptors = program.vsUniforms.size() + program.gsUniforms.size() + program.psUniforms.size() + program.csUniforms.size();
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descriptorHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    // define constants
    for (int32_t i = 0; i < program.vsUniforms.size(); i++) {
        Metadata::Uniform u = program.vsUniforms.at(i);

        D3D12_HEAP_PROPERTIES cbHeapProps = {};
        cbHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        cbHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        cbHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC cbResDesc = {};
        cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        cbResDesc.Width = u.size;
        cbResDesc.Height = 1;
        cbResDesc.DepthOrArraySize = 1;
        cbResDesc.MipLevels = 1;
        cbResDesc.Format = DXGI_FORMAT_UNKNOWN;
        cbResDesc.SampleDesc.Count = 1;
        cbResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        m_vsResources.emplace_back(nullptr);
        if (FAILED(device->CreateCommittedResource(
                &cbHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &cbResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_vsResources.back())))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
        cbvColorDesc.BufferLocation = m_vsResources.back()->GetGPUVirtualAddress();
        cbvColorDesc.SizeInBytes = u.size;
        device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
        heapHandle.ptr += unitSize;
    }
    for (int32_t i = 0; i < program.gsUniforms.size(); i++) {
        Metadata::Uniform u = program.gsUniforms.at(i);

        D3D12_HEAP_PROPERTIES cbHeapProps = {};
        cbHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        cbHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        cbHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        D3D12_RESOURCE_DESC cbResDesc = {};
        cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        cbResDesc.Width = u.size;
        cbResDesc.Height = 1;
        cbResDesc.DepthOrArraySize = 1;
        cbResDesc.MipLevels = 1;
        cbResDesc.Format = DXGI_FORMAT_UNKNOWN;
        cbResDesc.SampleDesc.Count = 1;
        cbResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        m_gsResources.emplace_back(nullptr);
        if (FAILED(device->CreateCommittedResource(
                &cbHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &cbResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_gsResources.back())))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
        cbvColorDesc.BufferLocation = m_gsResources.back()->GetGPUVirtualAddress();
        cbvColorDesc.SizeInBytes = u.size;
        device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
        heapHandle.ptr += unitSize;
    }
    for (int32_t i = 0; i < program.psUniforms.size(); i++) {
        Metadata::Uniform u = program.psUniforms.at(i);
        bool isShaderResource = u.type == Metadata::Uniform::Type::SRV;

        if (isShaderResource) {
            m_psResources.emplace_back(nullptr);
        } else {
            D3D12_HEAP_PROPERTIES cbHeapProps = {};
            cbHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            cbHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            cbHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            D3D12_RESOURCE_DESC cbResDesc = {};
            cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            cbResDesc.Width = u.size;
            cbResDesc.Height = 1;
            cbResDesc.DepthOrArraySize = 1;
            cbResDesc.MipLevels = 1;
            cbResDesc.Format = DXGI_FORMAT_UNKNOWN;
            cbResDesc.SampleDesc.Count = 1;
            cbResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            m_psResources.emplace_back(nullptr);
            if (FAILED(device->CreateCommittedResource(
                    &cbHeapProps,
                    D3D12_HEAP_FLAG_NONE,
                    &cbResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&m_psResources.back())))) {
                throw std::runtime_error("failed CreateCommittedResource()");
            }

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
            cbvColorDesc.BufferLocation = m_psResources.back()->GetGPUVirtualAddress();
            cbvColorDesc.SizeInBytes = u.size;
            device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
        }
        heapHandle.ptr += unitSize;
    }
    for (int32_t i = 0; i < program.csUniforms.size(); i++) {
        Metadata::Uniform u = program.csUniforms.at(i);

        switch (u.type) {
        case Metadata::Uniform::Type::CBV: {

            D3D12_HEAP_PROPERTIES cbHeapProps = {};
            cbHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            cbHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            cbHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            D3D12_RESOURCE_DESC cbResDesc = {};
            cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            cbResDesc.Width = u.size;
            cbResDesc.Height = 1;
            cbResDesc.DepthOrArraySize = 1;
            cbResDesc.MipLevels = 1;
            cbResDesc.Format = DXGI_FORMAT_UNKNOWN;
            cbResDesc.SampleDesc.Count = 1;
            cbResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            m_csResources.emplace_back(nullptr);
            if (FAILED(device->CreateCommittedResource(
                    &cbHeapProps,
                    D3D12_HEAP_FLAG_NONE,
                    &cbResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&m_csResources.back())))) {
                throw std::runtime_error("failed CreateCommittedResource()");
            }

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
            cbvColorDesc.BufferLocation = m_csResources.back()->GetGPUVirtualAddress();
            cbvColorDesc.SizeInBytes = u.size;
            device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
            break;
        }
        case Metadata::Uniform::Type::SRV:
            m_csResources.emplace_back(nullptr);
            break;
        case Metadata::Uniform::Type::UAV:
            m_csResources.emplace_back(nullptr);
            break;
        }
    }
}
}