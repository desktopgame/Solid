#include <Common/Graphics/Telop.hpp>
#include <Common/Graphics/TelopSystem.hpp>

namespace App::Common::Graphics {
std::vector<std::shared_ptr<Telop>> TelopSystem::s_freeTable;
std::vector<std::shared_ptr<Telop>> TelopSystem::s_usedTable;

void TelopSystem::initialize() { }

void TelopSystem::request(const std::string& text, const Vector3& position, const Vector3& color, float duration)
{
    if (s_freeTable.size() > 0) {
        std::shared_ptr<Telop> telop = s_freeTable.back();
        s_freeTable.pop_back();

        telop->initialize();
        s_usedTable.emplace_back(telop);
    } else {
        std::shared_ptr<Telop> telop = std::make_shared<Telop>();

        telop->initialize();
        s_usedTable.emplace_back(telop);
    }
}

void TelopSystem::draw()
{
    for (auto& telop : s_usedTable) {
        telop->update();
    }

    std::vector<std::shared_ptr<Telop>>& src = s_usedTable;
    std::vector<std::shared_ptr<Telop>>& dst = s_freeTable;

    for (auto& s : src) {
        if (s->isExpired()) {
            s->destroy();
            dst.emplace_back(s);
        }
    }

    auto iter = std::remove_if(src.begin(), src.end(), [](const auto& e) -> bool {
        return e->isExpired();
    });
    src.erase(iter, src.end());

    for (auto& telop : s_usedTable) {
        telop->update();
    }
}

void TelopSystem::stop()
{
    std::vector<std::shared_ptr<Telop>>& src = s_usedTable;
    std::vector<std::shared_ptr<Telop>>& dst = s_freeTable;

    for (auto& s : src) {
        s->destroy();
        dst.emplace_back(s);
    }

    src.clear();
}

void TelopSystem::destroy()
{
    s_freeTable.clear();
    s_usedTable.clear();
}
}