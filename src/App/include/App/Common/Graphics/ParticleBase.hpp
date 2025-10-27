#pragma once
#include <App/Common/Graphics/IParticle.hpp>
#include <App/library.hpp>
#include <cstdint>
#include <memory>

namespace App::Common::Graphics {
/**
 * パーティクルの共通パラメータを定義するクラスです。
 */
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

/**
 * パーティクルの基底クラスです。
 */
template <typename T, int32_t NumParticles>
class ParticleBase : public IParticle {
public:
    using Option = T;
    static inline constexpr IParticle::Type Type = IParticle::Type::None;

    explicit ParticleBase()
        : m_instanceBuffer()
        , m_particles()
        , m_indexLength()
        , m_initialized(false)
    {
    }
    virtual ~ParticleBase() = default;

    /**
     * パーティクルを初期化します。
     */
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

    void draw(const std::shared_ptr<CpuBuffer>& vertexBuffer, const std::shared_ptr<CpuBuffer>& indexBuffer, int32_t indexLength) override
    {
        auto surface = Engine::getInstance()->getDevice()->getSurface();
        Reflect::UCamera uCamera;
        uCamera.modelMatrix = Matrix::scale(m_scale);
        uCamera.viewMatrix = Camera::getLookAtMatrix();
        uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
        surface->uniformVS(m_uniformBuffer, 0, &uCamera);

        Reflect::UVector4 uColor;
        uColor.value = Vector4(m_color, 1.0f);
        surface->uniformVS(m_uniformBuffer, 1, &uColor);

        Reflect::UFloat uDeltatime;
        uDeltatime.value = Time::deltaTime();
        surface->uniformCS(m_uniformBuffer, 0, &uDeltatime);
        surface->uniformCS(m_uniformBuffer, 1, m_instanceBuffer->getGpuBuffer());

        auto rc = RenderContext::get(Metadata::ProgramTable::ParticleInstance3D);

        surface->sync(m_instanceBuffer);

        int32_t threads = (NumParticles + 255) / 256;
        surface->render(rc, m_uniformBuffer, vertexBuffer, indexBuffer, indexLength, std::vector<std::shared_ptr<IBuffer>> { m_instanceBuffer }, NumParticles, threads, 1, 1);
    }

    void destroy() override
    {
        if (m_uniformBuffer) {
            UniformPool::release(m_uniformBuffer);
            m_uniformBuffer = nullptr;
        }
    }

    bool isExpired() const override { return m_elapsed >= m_lifetime; }

protected:
    /**
     * パーティクルの初期値を設定します。
     * @param particles
     * @param params
     */
    virtual void batch(std::array<VertexParticle3D, NumParticles>& particles, const ParticleParameter<T>& params) = 0;

private:
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

        m_instanceBuffer = DualBuffer::create();
        m_instanceBuffer->allocate(sizeof(VertexParticle3D) * NumParticles);
    }
};
}