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
        return ub;
    } else {
        std::shared_ptr<UniformBuffer> ub = UniformBuffer::create(entry);
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
    std::vector<std::shared_ptr<UniformBuffer>>& src = s_usedTable.at(static_cast<int32_t>(entry));
    std::vector<std::shared_ptr<UniformBuffer>>& dst = s_freeTable.at(static_cast<int32_t>(entry));

    auto iter = std::remove(src.begin(), src.end(), ub);
    src.erase(iter, src.end());
    dst.emplace_back(ub);
    ub->reset();
}

void UniformPool::releaseAll()
{
    for (int32_t i = 0; i < Metadata::ProgramTable::Count; i++) {
        std::vector<std::shared_ptr<UniformBuffer>>& src = s_usedTable.at(i);
        std::vector<std::shared_ptr<UniformBuffer>>& dst = s_freeTable.at(i);

        for (const auto& s : src) {
            if (!s->isOwned()) {
                dst.emplace_back(s);
            }
        }

        auto iter = std::remove_if(src.begin(), src.end(), [](const auto& e) -> bool {
            return !e->isOwned();
        });
        src.erase(iter, src.end());
    }
}
// internal
void UniformPool::initialize()
{
}

void UniformPool::destroy()
{
    s_freeTable.clear();
    s_usedTable.clear();
}
// private
}