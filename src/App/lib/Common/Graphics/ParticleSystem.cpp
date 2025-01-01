#include <Common/Graphics/ParticleSystem.hpp>

namespace App::Common::Graphics {
std::vector<std::vector<std::shared_ptr<IParticle>>> ParticleSystem::s_freeTable(static_cast<int32_t>(IParticle::Type::Count));
std::vector<std::vector<std::shared_ptr<IParticle>>> ParticleSystem::s_usedTable(static_cast<int32_t>(IParticle::Type::Count));
void ParticleSystem::initialize()
{
}

void ParticleSystem::draw()
{
    for (int32_t i = 0; i < static_cast<int32_t>(IParticle::Type::Count); i++) {
        std::vector<std::shared_ptr<IParticle>>& particles = s_usedTable.at(i);

        for (auto& particle : particles) {
            particle->update();
        }
    }

    for (int32_t i = 0; i < static_cast<int32_t>(IParticle::Type::Count); i++) {
        std::vector<std::shared_ptr<IParticle>>& src = s_usedTable.at(i);
        std::vector<std::shared_ptr<IParticle>>& dst = s_freeTable.at(i);

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
    }

    for (int32_t i = 0; i < static_cast<int32_t>(IParticle::Type::Count); i++) {
        std::vector<std::shared_ptr<IParticle>>& particles = s_usedTable.at(i);

        for (auto& particle : particles) {
            particle->draw();
        }
    }
}

void ParticleSystem::stop()
{
    for (int32_t i = 0; i < static_cast<int32_t>(IParticle::Type::Count); i++) {
        std::vector<std::shared_ptr<IParticle>>& src = s_usedTable.at(i);
        std::vector<std::shared_ptr<IParticle>>& dst = s_freeTable.at(i);

        for (auto& s : src) {
            s->destroy();
            dst.emplace_back(s);
        }

        src.clear();
    }
}

void ParticleSystem::destroy()
{
    s_freeTable.clear();
    s_usedTable.clear();
}
}