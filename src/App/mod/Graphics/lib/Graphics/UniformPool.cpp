#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Metadata.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/UniformPool.hpp>

namespace Lib::Graphics {
const int32_t UniformPool::k_bufferUnitSize = 256000000; // 256MB
Microsoft::WRL::ComPtr<ID3D12Resource> UniformPool::s_globalBuffer;
void* UniformPool::s_globalBufferMemory;
uint64_t UniformPool::s_globalBufferOffset;
bool UniformPool::s_globalBufferFlip;

std::vector<std::vector<std::shared_ptr<UniformBuffer>>> UniformPool::s_freeTable(Metadata::ProgramTable::Count);
std::vector<std::vector<std::shared_ptr<UniformBuffer>>> UniformPool::s_usedTable(Metadata::ProgramTable::Count);
// public
std::shared_ptr<UniformBuffer> UniformPool::rent(Metadata::ProgramTable entry)
{
    std::vector<std::shared_ptr<UniformBuffer>>& freeVec = s_freeTable.at(entry);
    D3D12_GPU_VIRTUAL_ADDRESS gpuAddr = s_globalBuffer->GetGPUVirtualAddress();
    gpuAddr += s_globalBufferOffset;

    unsigned char* cpuAddr = static_cast<unsigned char*>(s_globalBufferMemory);
    cpuAddr += s_globalBufferOffset;

    if (freeVec.size() > 0) {
        std::shared_ptr<UniformBuffer> ub = freeVec.back();
        ub->bufferLocation(gpuAddr, cpuAddr);
        s_globalBufferOffset += progressBufferOffset(entry);

        freeVec.pop_back();
        s_usedTable.at(entry).emplace_back(ub);
        return ub;
    } else {
        std::shared_ptr<UniformBuffer> ub = UniformBuffer::create(entry);
        ub->bufferLocation(gpuAddr, cpuAddr);
        s_globalBufferOffset += progressBufferOffset(entry);

        s_usedTable.at(entry).emplace_back(ub);
        return ub;
    }
    return nullptr;
}

void UniformPool::release(const std::shared_ptr<UniformBuffer>& ub)
{
    if (!ub->isOwned()) {
        throw std::logic_error("uniform buffer will auto released.");
    }
    Metadata::ProgramTable entry = ub->getEntry();
    std::vector<std::shared_ptr<UniformBuffer>>& src = s_usedTable.at(entry);
    std::vector<std::shared_ptr<UniformBuffer>>& dst = s_freeTable.at(entry);

    auto iter = std::remove(src.begin(), src.end(), ub);
    src.erase(iter, src.end());
    dst.emplace_back(ub);
    ub->reset();
}

void UniformPool::releaseAll()
{
    uint32_t skip = 0;
    for (int32_t i = 0; i < Metadata::ProgramTable::Count; i++) {
        std::vector<std::shared_ptr<UniformBuffer>>& src = s_usedTable.at(i);
        std::vector<std::shared_ptr<UniformBuffer>>& dst = s_freeTable.at(i);

        for (const auto& s : src) {
            if (!s->isOwned()) {
                dst.emplace_back(s);
            } else {
                skip += progressBufferOffset(static_cast<Metadata::ProgramTable>(i));
            }
        }

        auto iter = std::remove_if(src.begin(), src.end(), [](const auto& e) -> bool {
            return !e->isOwned();
        });
        src.erase(iter, src.end());
    }

    s_globalBufferFlip = !s_globalBufferFlip;
    s_globalBufferOffset = (s_globalBufferFlip ? k_bufferUnitSize : 0) + skip;
}
// internal
void UniformPool::initialize()
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();

    D3D12_HEAP_PROPERTIES cbHeapProps = {};
    cbHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    cbHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    cbHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    D3D12_RESOURCE_DESC cbResDesc = {};
    cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cbResDesc.Width = k_bufferUnitSize * 2; // 描画中は空いてるもう半分に書きこむのでx2
    cbResDesc.Height = 1;
    cbResDesc.DepthOrArraySize = 1;
    cbResDesc.MipLevels = 1;
    cbResDesc.Format = DXGI_FORMAT_UNKNOWN;
    cbResDesc.SampleDesc.Count = 1;
    cbResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    if (FAILED(device->CreateCommittedResource(
            &cbHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &cbResDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&s_globalBuffer)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }

    // アプリが終了するまでUnmapしない。
    // GPUが読むときも特にUnmapする必要はないらしい。
    s_globalBuffer->Map(0, nullptr, (void**)&s_globalBufferMemory);
}

void UniformPool::destroy()
{
    s_globalBuffer->Unmap(0, nullptr);
    s_freeTable.clear();
    s_usedTable.clear();
}
// private
uint32_t UniformPool::progressBufferOffset(Metadata::ProgramTable entry)
{
    uint32_t offset = 0;
    const Metadata::Program& program = Metadata::k_programs.at(static_cast<int>(entry));
    for (const auto& vu : program.vsUniforms) {
        if (vu.type == Metadata::Uniform::Type::CBV) {
            offset += vu.size;
        }
    }
    for (const auto& gu : program.gsUniforms) {
        if (gu.type == Metadata::Uniform::Type::CBV) {
            offset += gu.size;
        }
    }
    for (const auto& pu : program.psUniforms) {
        if (pu.type == Metadata::Uniform::Type::CBV) {
            offset += pu.size;
        }
    }
    for (const auto& cu : program.csUniforms) {
        if (cu.type == Metadata::Uniform::Type::CBV) {
            offset += cu.size;
        }
    }
    return offset;
}
}