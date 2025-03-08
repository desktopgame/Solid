#pragma once
#include <App/Common/Graphics/ParticleBase.hpp>
#include <App/library.hpp>
#include <random>

namespace App::Common::Graphics {

struct PlaneParticleOption {
    Vector3 floorPoint;
    Vector3 size;

    PlaneParticleOption(const Vector3& floorPoint, Vector3 size)
        : floorPoint(floorPoint)
        , size(size)
    {
    }
};

class PlaneParticle : public ParticleBase<PlaneParticleOption, 128> {
public:
    using Option = PlaneParticleOption;
    static inline constexpr IParticle::Type Type = IParticle::Type::Plane;

    explicit PlaneParticle()
        : ParticleBase()
    {
    }

protected:
    void batch(std::array<VertexParticle3D, 128>& particles, const ParticleParameter<PlaneParticleOption>& params) override
    {
        Random random;
        for (auto& particle : particles) {
            float fx = random.range(-(params.options.size.x() / 2.0f), (params.options.size.x() / 2.0f));
            float fz = random.range(-(params.options.size.z() / 2.0f), (params.options.size.z() / 2.0f));
            float speed = 40.5f;
            Vector3 direction = Vector3({ 0, 1, 0 });

            particle.offset = params.options.floorPoint + Vector3({ fx, 0, fz });
            particle.velocity = direction * speed;
            particle.lifetime = random.range(params.minLifetime, params.maxLifetime);
        }
    }
};
}