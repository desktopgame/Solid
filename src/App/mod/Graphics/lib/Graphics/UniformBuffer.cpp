#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
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
void UniformBuffer::setPS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).psUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    Metadata::Uniform u = Metadata::k_programs.at(m_entry).psUniforms.at(index);
    if (u.isShaderResource) {
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
    if (u.isShaderResource) {
        auto device = Engine::getInstance()->getDevice()->getID3D12Device();
        uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        heapHandle.ptr += Metadata::k_programs.at(m_entry).vsUniforms.size() * unitSize;
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
// internal
std::shared_ptr<UniformBuffer> UniformBuffer::create(Metadata::ProgramTable entry)
{
    auto ub = std::shared_ptr<UniformBuffer>(new UniformBuffer());
    ub->m_entry = entry;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    // descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NodeMask = 0;
    descHeapDesc.NumDescriptors = Metadata::k_programs.at(entry).vsUniforms.size() + Metadata::k_programs.at(entry).psUniforms.size();
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&ub->m_descriptorHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = ub->m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    // define constants
    for (int32_t i = 0; i < Metadata::k_programs.at(entry).vsUniforms.size(); i++) {
        Metadata::Uniform u = Metadata::k_programs.at(entry).vsUniforms.at(i);

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

        ub->m_vsResources.emplace_back(nullptr);
        if (FAILED(device->CreateCommittedResource(
                &cbHeapProps,
                D3D12_HEAP_FLAG_NONE,
                &cbResDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&ub->m_vsResources.back())))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
        cbvColorDesc.BufferLocation = ub->m_vsResources.back()->GetGPUVirtualAddress();
        cbvColorDesc.SizeInBytes = u.size;
        device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
        heapHandle.ptr += unitSize;
    }
    for (int32_t i = 0; i < Metadata::k_programs.at(entry).psUniforms.size(); i++) {
        Metadata::Uniform u = Metadata::k_programs.at(entry).psUniforms.at(i);

        if (u.isShaderResource) {
            ub->m_psResources.emplace_back(nullptr);
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

            ub->m_psResources.emplace_back(nullptr);
            if (FAILED(device->CreateCommittedResource(
                    &cbHeapProps,
                    D3D12_HEAP_FLAG_NONE,
                    &cbResDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&ub->m_psResources.back())))) {
                throw std::runtime_error("failed CreateCommittedResource()");
            }

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
            cbvColorDesc.BufferLocation = ub->m_psResources.back()->GetGPUVirtualAddress();
            cbvColorDesc.SizeInBytes = u.size;
            device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
        }
        heapHandle.ptr += unitSize;
    }
    return ub;
}

void UniformBuffer::destroy()
{
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UniformBuffer::getID3D12DescriptorHeap() const
{
    return m_descriptorHeap;
}
// private
UniformBuffer::UniformBuffer()
{
}
}