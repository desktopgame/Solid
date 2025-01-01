#pragma once
#include <Common/Graphics/IParticle.hpp>
#include <Common/Graphics/ParticleBase.hpp>
#include <memory>
#include <vector>

namespace App::Common::Graphics {
class ParticleSystem {
public:
    static void initialize();

    template <typename T>
    static std::shared_ptr<IParticle> emit(const ParticleParameter<typename T::Option>& params)
    {
        static_assert(std::is_base_of_v<ParticleBase<typename T::Option>, T>, "T must derive from ParticleBase<Option>");

        int32_t typeIndex = static_cast<int32_t>(T::Type);
        std::vector<std::shared_ptr<IParticle>>& freeVec = s_freeTable.at(typeIndex);
        if (freeVec.size() > 0) {
            std::shared_ptr<IParticle> particle = freeVec.back();
            freeVec.pop_back();
            s_usedTable.at(typeIndex).emplace_back(particle);

            std::static_pointer_cast<T>(particle)->initialize(params);
            return particle;
        } else {
            std::shared_ptr<IParticle> particle = std::make_shared<T>();
            s_usedTable.at(typeIndex).emplace_back(particle);

            std::static_pointer_cast<T>(particle)->initialize(params);
            return particle;
        }
        return nullptr;
    }

    static void draw();

    static void destroy();

private:
    ParticleSystem() = delete;

    static std::vector<std::vector<std::shared_ptr<IParticle>>> s_freeTable;
    static std::vector<std::vector<std::shared_ptr<IParticle>>> s_usedTable;
};
}