#pragma once
#include <Common/Graphics/ParticleBase.hpp>
#include <library.hpp>
#include <random>

namespace App::Common::Graphics {

struct SphericalParticleOption {
    Vector3 collisionPoint;
    float minSpeed;
    float maxSpeed;

    SphericalParticleOption(const Vector3& point, float minSpeed, float maxSpeed)
        : collisionPoint(point)
        , minSpeed(minSpeed)
        , maxSpeed(maxSpeed)
    {
    }
};

class SphericalParticle : public ParticleBase<SphericalParticleOption> {
public:
    using Option = SphericalParticleOption;
    static inline constexpr IParticle::Type Type = IParticle::Type::Spherical;

    explicit SphericalParticle()
        : ParticleBase()
    {
    }

protected:
    void batch(std::array<VertexParticle3D, NumParticles>& particles, const ParticleParameter<SphericalParticleOption>& params) override
    {
        Random random;
        for (auto& particle : particles) {
            float fx = random.range(-1.0f, 1.0f);
            float fy = random.range(-1.0f, 1.0f);
            float fz = random.range(-1.0f, 1.0f);
            float speed = random.range(params.options.minSpeed, params.options.maxSpeed);
            Vector3 direction = Vector3::normalized(Vector3({ fx, fy, fz }));

            particle.offset = params.options.collisionPoint;
            particle.velocity = direction * speed;
            particle.lifetime = random.range(params.minLifetime, params.maxLifetime);
        }
    }
};
}