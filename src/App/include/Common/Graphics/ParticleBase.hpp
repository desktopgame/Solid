#pragma once
#include <Common/Graphics/IParticle.hpp>
#include <cstdint>
#include <library.hpp>
#include <memory>

namespace App::Common::Graphics {
template <typename T>
class ParticleParameter {
public:
    Vector3 color;
    Vector3 scale;
    float minLifetime;
    float maxLifetime;
    T options;

    explicit ParticleParameter(
        const Vector3& color,
        const Vector3& scale,
        float minLifetime,
        float maxLifetime,
        const T& options)
        : color(color)
        , scale(scale)
        , minLifetime(minLifetime)
        , maxLifetime(maxLifetime)
        , options(options)
    {
    }
};

template <typename T>
class ParticleBase : public IParticle {
public:
    static inline constexpr int32_t NumParticles = 4096;
    using OptionType = T;

    explicit ParticleBase()
        : m_vertexBuffer()
        , m_indexBuffer()
        , m_instanceBuffer()
        , m_particles()
        , m_indexLength()
        , m_initialized(false)
    {
    }
    virtual ~ParticleBase() = default;

    void initialize(const ParticleParameter<T>& params)
    {
        baseInit();

        batch(m_particles, params);
        m_color = params.color;
        m_scale = params.scale;
        m_lifetime = params.maxLifetime;
        m_elapsed = 0.0f;
        m_instanceBuffer->update(m_particles.data());
        m_uniformBuffer = UniformPool::rent(Metadata::ProgramTable::ParticleInstance3D)->owned();
    }

    void update() override
    {
        if (!isExpired()) {
            m_elapsed += Time::deltaTime();
        }
    }

    void draw() override
    {
        Reflect::UCamera uCamera;
        uCamera.modelMatrix = Matrix::scale(m_scale);
        uCamera.viewMatrix = Camera::getLookAtMatrix();
        uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
        m_uniformBuffer->setVS(0, &uCamera);

        Reflect::UVector4 uColor;
        uColor.value = Vector4(m_color, 1.0f);
        m_uniformBuffer->setVS(1, &uColor);

        Reflect::UFloat uDeltatime;
        uDeltatime.value = Time::deltaTime();
        m_uniformBuffer->setCS(0, &uDeltatime);
        m_uniformBuffer->setCS(1, m_instanceBuffer->getGpuBuffer());

        auto rc = RenderContext::get(Metadata::ProgramTable::ParticleInstance3D);

        auto surface = Engine::getInstance()->getDevice()->getSurface();
        surface->sync(m_instanceBuffer);

        int32_t threads = (NumParticles + 255) / 256;
        surface->render(rc, m_uniformBuffer, m_vertexBuffer, m_indexBuffer, m_indexLength, std::vector<std::shared_ptr<IBuffer>> { m_instanceBuffer }, NumParticles, threads, 1, 1);
    }

    void destroy() override
    {
        if (m_uniformBuffer) {
            UniformPool::release(m_uniformBuffer);
            m_uniformBuffer = nullptr;
        }
    }

    bool isExpired() const override { return m_elapsed >= m_lifetime; }

    virtual IParticle::Type getType() const { return IParticle::Type::None; }

protected:
    virtual void batch(std::array<VertexParticle3D, NumParticles>& particles, const ParticleParameter<T>& params) = 0;

private:
    std::shared_ptr<CpuBuffer> m_vertexBuffer;
    std::shared_ptr<CpuBuffer> m_indexBuffer;
    std::shared_ptr<DualBuffer> m_instanceBuffer;
    std::array<VertexParticle3D, NumParticles> m_particles;
    std::shared_ptr<UniformBuffer> m_uniformBuffer;
    int32_t m_indexLength;
    Vector3 m_color;
    Vector3 m_scale;
    float m_lifetime;
    float m_elapsed;
    bool m_initialized;

    void baseInit()
    {
        if (m_initialized) {
            return;
        }
        m_initialized = true;

        m_vertexBuffer = CpuBuffer::create();
        m_indexBuffer = CpuBuffer::create();
        m_instanceBuffer = DualBuffer::create();

        std::vector<VertexNormal3D> vertices;
        std::vector<uint32_t> indices;
        Polygon::generateBox(vertices, indices);

        m_vertexBuffer->allocate(sizeof(VertexNormal3D) * vertices.size());
        m_vertexBuffer->update(vertices.data());

        m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
        m_indexBuffer->update(indices.data());
        m_indexLength = static_cast<int32_t>(indices.size());

        m_instanceBuffer->allocate(sizeof(VertexNormal3D) * NumParticles);
    }
};
}