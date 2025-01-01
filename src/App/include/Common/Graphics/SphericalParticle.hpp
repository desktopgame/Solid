#pragma once
#include <Common/Graphics/Particle.hpp>
#include <library.hpp>
#include <random>

namespace App::Common::Graphics {

struct SphericalParticleOption {
    Vector3 collisionPoint;
    float minSpeed;
    float maxSpeed;

    SphericalParticleOption(const Vector3& point, float min, float max)
        : collisionPoint(point)
        , minSpeed(min)
        , maxSpeed(max)
    {
    }
};

class SphericalParticle : public Particle<SphericalParticleOption> {
public:
    explicit SphericalParticle()
        : Particle()
    {
    }

protected:
    void batch(std::array<VertexParticle3D, NumParticles>& particles, const SphericalParticleOption& option) override
    {
        Random random;
        for (auto& particle : particles) {
            float fx = random.range(-1.0f, 1.0f);
            float fy = random.range(-1.0f, 1.0f);
            float fz = random.range(-1.0f, 1.0f);
            Vector3 direction = Vector3::normalized(Vector3({ fx, fy, fz }));

            particle.offset = option.collisionPoint;
            particle.velocity = direction;
        }
    }
};
}