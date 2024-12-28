#pragma once
#include <Common/Graphics/Node.hpp>
#include <Scenes/Game/System/Entity.hpp>

namespace App::Scenes::Game::System::Entities {
class BasicEntity : public Entity {
public:
    static std::shared_ptr<BasicEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~BasicEntity();

    virtual void update(Field& field) override;
    virtual void onGui() override;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) override;
    virtual void onHitStayEntity(const std::shared_ptr<Entity>& entity) override;
    virtual void onHitExitEntity(const std::shared_ptr<Entity>& entity) override;

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

    virtual void onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z);
    virtual void onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z);
    virtual void onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z);
    virtual void onRotationStop(Field& field);

private:
    class NodeHit {
    public:
        std::shared_ptr<Common::Graphics::Node> node;
        IntVector3 tile;
    };

    void rehashAABB(const std::shared_ptr<Common::Graphics::Node>& node, Geom::AABB& dst);
    void hitTilesFuzzy(Field& field, const Vector3& offset, std::vector<IntVector3>& hits);
    void hitTilesStrict(Field& field, const std::shared_ptr<Common::Graphics::Node>& node, const Vector3& offset, std::vector<IntVector3>& checkTiles, std::vector<NodeHit>& hits);
    static float alignTile(float a, float tileSize);

    std::shared_ptr<Common::Graphics::Node> m_node;
    IntVector3 m_groundTile;
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