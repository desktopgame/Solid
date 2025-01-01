#pragma once
#include <Graphics/Metadata.hpp>
#include <memory>
#include <vector>

namespace Lib::Graphics {
class UniformBuffer;
class UniformPool {
public:
    static std::shared_ptr<UniformBuffer> rent(Metadata::ProgramTable entry);
    static void release(const std::shared_ptr<UniformBuffer>& ub);
    static void releaseAll();

#if SOLID_ENABLE_INTERNAL
    static void initialize();
    static void destroy();
#endif
private:
    UniformPool() = delete;

    static std::vector<std::vector<std::shared_ptr<UniformBuffer>>> s_freeTable;
    static std::vector<std::vector<std::shared_ptr<UniformBuffer>>> s_usedTable;
};
}