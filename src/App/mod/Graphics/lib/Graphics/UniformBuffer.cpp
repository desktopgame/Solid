#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/GpuBuffer.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/UniformBuffer.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
Metadata::ProgramTable UniformBuffer::getEntry() const { return m_entry; }
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

void UniformBuffer::bufferLocation(D3D12_GPU_VIRTUAL_ADDRESS gpuBufferLocation, unsigned char* cpuBufferLocation)
{
    m_gpuBufferLocation = gpuBufferLocation;
    m_cpuBufferLocation = cpuBufferLocation;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    const Metadata::Program& program = Metadata::k_programs.at(m_entry);
    uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    // 頂点シェーダーの定数バッファを確保
    for (int32_t i = 0; i < program.vsUniforms.size(); i++) {
        Metadata::Uniform u = program.vsUniforms.at(i);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = gpuBufferLocation;
        cbvDesc.SizeInBytes = u.size;
        device->CreateConstantBufferView(&cbvDesc, heapHandle);
        heapHandle.ptr += unitSize;
        gpuBufferLocation += u.size;
    }
    // ジオメトリシェーダの定数バッファを確保
    for (int32_t i = 0; i < program.gsUniforms.size(); i++) {
        Metadata::Uniform u = program.gsUniforms.at(i);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = gpuBufferLocation;
        cbvDesc.SizeInBytes = u.size;
        device->CreateConstantBufferView(&cbvDesc, heapHandle);
        heapHandle.ptr += unitSize;
        gpuBufferLocation += u.size;
    }
    // ピクセルシェーダーの定数バッファを確保
    for (int32_t i = 0; i < program.psUniforms.size(); i++) {
        Metadata::Uniform u = program.psUniforms.at(i);
        bool isShaderResource = u.type == Metadata::Uniform::Type::SRV;

        if (isShaderResource) {
            // m_psResources.emplace_back(nullptr);
        } else {
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
            cbvDesc.BufferLocation = gpuBufferLocation;
            cbvDesc.SizeInBytes = u.size;
            device->CreateConstantBufferView(&cbvDesc, heapHandle);
            gpuBufferLocation += u.size;
        }
        heapHandle.ptr += unitSize;
    }
    // コンピュートシェーダの定数バッファを確保
    for (int32_t i = 0; i < program.csUniforms.size(); i++) {
        Metadata::Uniform u = program.csUniforms.at(i);

        switch (u.type) {
        case Metadata::Uniform::Type::CBV: {
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
            cbvDesc.BufferLocation = gpuBufferLocation;
            cbvDesc.SizeInBytes = u.size;
            device->CreateConstantBufferView(&cbvDesc, heapHandle);
            gpuBufferLocation += u.size;
            break;
        }
        case Metadata::Uniform::Type::SRV:
            // m_csResources.emplace_back(nullptr);
            break;
        case Metadata::Uniform::Type::UAV:
            // m_csResources.emplace_back(nullptr);
            break;
        }
    }
}

void UniformBuffer::setVS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).vsUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    unsigned char* memory = static_cast<unsigned char*>(m_cpuBufferLocation);
    for (int32_t i = 0; i < index; i++) {
        const auto& vu = Metadata::k_programs.at(m_entry).vsUniforms.at(i);
        if (vu.type == Metadata::Uniform::Type::CBV) {
            memory += vu.size;
        }
    }
    ::memcpy(memory, data, Metadata::k_programs.at(m_entry).vsUniforms.at(index).size);
}

void UniformBuffer::setGS(int32_t index, const void* data)
{
    if (index >= Metadata::k_programs.at(m_entry).gsUniforms.size()) {
        throw std::logic_error("uniform is out of range.");
    }
    unsigned char* memory = static_cast<unsigned char*>(m_cpuBufferLocation);
    for (const auto& vu : Metadata::k_programs.at(m_entry).vsUniforms) {
        if (vu.type == Metadata::Uniform::Type::CBV) {
            memory += vu.size;
        }
    }
    for (int32_t i = 0; i < index; i++) {
        const auto& gu = Metadata::k_programs.at(m_entry).gsUniforms.at(i);
        if (gu.type == Metadata::Uniform::Type::CBV) {
            memory += gu.size;
        }
    }
    ::memcpy(memory, data, Metadata::k_programs.at(m_entry).gsUniforms.at(index).size);
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
        unsigned char* memory = static_cast<unsigned char*>(m_cpuBufferLocation);
        for (const auto& vu : Metadata::k_programs.at(m_entry).vsUniforms) {
            if (vu.type == Metadata::Uniform::Type::CBV) {
                memory += vu.size;
            }
        }
        for (const auto& gu : Metadata::k_programs.at(m_entry).gsUniforms) {
            if (gu.type == Metadata::Uniform::Type::CBV) {
                memory += gu.size;
            }
        }
        for (int32_t i = 0; i < index; i++) {
            const auto& pu = Metadata::k_programs.at(m_entry).psUniforms.at(i);
            if (pu.type == Metadata::Uniform::Type::CBV) {
                memory += pu.size;
            }
        }
        ::memcpy(memory, data, Metadata::k_programs.at(m_entry).psUniforms.at(index).size);
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
    unsigned char* memory = static_cast<unsigned char*>(m_cpuBufferLocation);
    for (const auto& vu : Metadata::k_programs.at(m_entry).vsUniforms) {
        if (vu.type == Metadata::Uniform::Type::CBV) {
            memory += vu.size;
        }
    }
    for (const auto& gu : Metadata::k_programs.at(m_entry).gsUniforms) {
        if (gu.type == Metadata::Uniform::Type::CBV) {
            memory += gu.size;
        }
    }
    for (const auto& pu : Metadata::k_programs.at(m_entry).psUniforms) {
        if (pu.type == Metadata::Uniform::Type::CBV) {
            memory += pu.size;
        }
    }
    for (int32_t i = 0; i < index; i++) {
        const auto& cu = Metadata::k_programs.at(m_entry).csUniforms.at(i);
        if (cu.type == Metadata::Uniform::Type::CBV) {
            memory += cu.size;
        }
    }
    ::memcpy(memory, data, Metadata::k_programs.at(m_entry).csUniforms.at(index).size);
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

void UniformBuffer::reset() { }

void UniformBuffer::beginCompute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    for (auto uavBuffer : m_uavBuffers) {
        if (uavBuffer) {
            uavBuffer->beginCompute(cmdList);
        }
    }
}
void UniformBuffer::endCompute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    for (auto uavBuffer : m_uavBuffers) {
        if (uavBuffer) {
            uavBuffer->endCompute(cmdList);
        }
    }
}
void UniformBuffer::syncCompute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
    for (auto uavBuffer : m_uavBuffers) {
        if (uavBuffer) {
            uavBuffer->syncCompute(cmdList);
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
    , m_uavBuffers()
{
}

void UniformBuffer::init(Metadata::ProgramTable entry)
{
    m_entry = entry;

    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    const Metadata::Program& program = Metadata::k_programs.at(entry);
    // プログラムで指定された全てのユニフォーム分のディスクリプターを確保する
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NodeMask = 0;
    descHeapDesc.NumDescriptors = program.vsUniforms.size() + program.gsUniforms.size() + program.psUniforms.size() + program.csUniforms.size();
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descriptorHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
}
}