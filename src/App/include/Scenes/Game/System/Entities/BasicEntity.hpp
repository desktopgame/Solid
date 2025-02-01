#pragma once
#include <Common/Graphics/Node.hpp>
#include <Scenes/Game/System/Entity.hpp>

namespace App::Scenes::Game::System::Entities {
class BasicEntity : public Entity {
public:
    static std::shared_ptr<BasicEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~BasicEntity();

    virtual void idle(const std::shared_ptr<Chunk>& chunk);
    virtual void update(const std::shared_ptr<Chunk>& chunk) override;
    virtual void onGui() override;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) override;
    virtual void onHitStayEntity(const std::shared_ptr<Entity>& entity) override;
    virtual void onHitExitEntity(const std::shared_ptr<Entity>& entity) override;
    virtual bool damage(const std::shared_ptr<DamageSource>& damageSource) override;

    void knockback(const Vector3& direction, float speed, float duration);

    std::shared_ptr<Common::Graphics::Node> getNode() const;
    Geom::OBB getOBB() const override;

    void markAsDirtyAABB();
    void rehashAABB();
    Geom::AABB getAABB() const override;

    void setVelocity(const Vector3& velocity);
    Vector3 getVelocity() const;

    void setTorque(const Vector3& torque);
    Vector3 getTorque() const;

    void setReceiveGravity(bool receiveGravity);
    bool isReceiveGravity() const;

    bool isOnGround() const;

protected:
    BasicEntity(const std::shared_ptr<Common::Graphics::Node>& node);
    void onPositionChanged(const Vector3& position) override;
    void onRotationChanged(const Vector3& rotation) override;

    virtual void onCollisionWall(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z);
    virtual void onCollisionRoof(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z);
    virtual void onCollisionFloor(const std::shared_ptr<Chunk>& chunk, int32_t x, int32_t y, int32_t z);
    virtual void onRotationStop(const std::shared_ptr<Chunk>& chunk);

private:
    class NodeHit {
    public:
        std::shared_ptr<Common::Graphics::Node> node;
        IntVector3 tile;
    };

    void rehashAABB(const std::shared_ptr<Common::Graphics::Node>& node, Geom::AABB& dst);
    void hitTilesFuzzy(const std::shared_ptr<Chunk>& chunk, const Vector3& offset, std::vector<IntVector3>& hits);
    void hitTilesStrict(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Common::Graphics::Node>& node, const Vector3& offset, std::vector<IntVector3>& checkTiles, std::vector<NodeHit>& hits);

    static inline float alignTile(float a, float tileSize)
    {
        float tileHalf = tileSize / 2.0f;
        float d = a / tileSize;
        if (d > 0.0f) {
            d = ::floorf(d);
        } else {
            d = ::ceilf(d);
        }
        float m = ::fmodf(a, tileSize);

        if (::fabs(m) < 0.000001f) {
            return a;
        }
        if (m > 0.0f) {
            if (m < tileHalf) {
                return d * tileSize;
            }
            return (d * tileSize) + tileSize;
        } else {
            if (::fabs(m) < tileHalf) {
                return d * tileSize;
            }
            return (d * tileSize) - tileSize;
        }
    }

    std::shared_ptr<Common::Graphics::Node> m_node;
    IntVector3 m_groundTile;
    Vector3 m_knockbackDirection;
    float m_knockbackSpeed;
    float m_knockbackElapsed;
    float m_knockbackDuration;
    bool m_knockbackPlaying;
    float m_damageElapsed;
    float m_damageDuration;
    bool m_damagePlaying;
    Geom::AABB m_aabb;
    bool m_dirtyAABB;
    Vector3 m_velocity;
    Vector3 m_torque;
    bool m_receiveGravity;
    bool m_onGround;
    std::vector<IntVector3> m_fuzzyHitCache;
    std::vector<NodeHit> m_strictHitCache;
};
}