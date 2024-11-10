#include <Graphics/Metadata.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/UniformPool.hpp>

namespace Lib::Graphics {
std::vector<std::vector<std::shared_ptr<UniformBuffer>>> UniformPool::s_freeTable(Metadata::ProgramTable::Count);
std::vector<std::vector<std::shared_ptr<UniformBuffer>>> UniformPool::s_usedTable(Metadata::ProgramTable::Count);
// public
std::shared_ptr<UniformBuffer> UniformPool::rent(Metadata::ProgramTable entry)
{
    std::vector<std::shared_ptr<UniformBuffer>>& freeVec = s_freeTable.at(entry);
    if (freeVec.size() > 0) {
        std::shared_ptr<UniformBuffer> ub = freeVec.back();
        freeVec.pop_back();
        s_usedTable.at(entry).emplace_back(ub);
    } else {
        std::shared_ptr<UniformBuffer> ub = UniformBuffer::create(entry);
        s_usedTable.at(entry).emplace_back(ub);
        return ub;
    }
    return nullptr;
}

void UniformPool::releaseAll()
{
    for (int32_t i = 0; i < Metadata::ProgramTable::Count; i++) {
        std::vector<std::shared_ptr<UniformBuffer>>& src = s_usedTable.at(i);
        std::vector<std::shared_ptr<UniformBuffer>>& dst = s_freeTable.at(i);

        for (const auto& s : src) {
            dst.emplace_back(s);
        }
        src.clear();
    }
}
// internal
void UniformPool::initialize()
{
}

void UniformPool::destroy()
{
}
// private
}