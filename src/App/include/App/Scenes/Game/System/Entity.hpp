#pragma once
#include <App/library.hpp>
#include <memory>
#include <stduuid/uuid.h>
#include <unordered_map>

namespace App::Scenes::Game::System {
class Field;
class Chunk;
class DamageSource;
class Entity : public std::enable_shared_from_this<Entity> {
public:
    enum class Category : uint32_t {
        None = 0,
        // プレイヤー
        Player,
        // プレイヤー陣営（プレイヤーの発射した弾丸など）
        PlayerTeam,
        // エネミー
        Enemy,
        // エネミー陣営（エネミーの発射した弾丸など）
        EnemyTeam
    };

    virtual ~Entity();

    virtual void update(const std::shared_ptr<Chunk>& chunk) = 0;
    virtual void onGui() = 0;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) = 0;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) = 0;

    void addHitEntity(const std::shared_ptr<Entity>& entity);
    void removeHitEntity(const std::shared_ptr<Entity>& entity);
    bool isHitOnEntity(const std::shared_ptr<Entity>& entity) const;
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) = 0;
    virtual void onHitStayEntity(const std::shared_ptr<Entity>& entity) = 0;
    virtual void onHitExitEntity(const std::shared_ptr<Entity>& entity) = 0;

    virtual bool damage(const std::shared_ptr<DamageSource>& damageSource);

    virtual Geom::OBB getOBB() const;
    virtual Geom::AABB getAABB() const;

    int32_t getMaximumHP() const;
    int32_t getCurrentHP() const;
    bool isDead() const;

    void setPosition(const Vector3& position);
    Vector3 getPosition() const;

    void setRotation(const Vector3& rotation);
    Vector3 getRotation() const;

    Category getCategory() const;

    const uuids::uuid& getUuid() const;

protected:
    Entity();

    virtual void onDead(const std::shared_ptr<DamageSource>& damageSource);
    virtual void onPositionChanged(const Vector3& position);
    virtual void onRotationChanged(const Vector3& rotation);

    std::unordered_map<uuids::uuid, std::shared_ptr<Entity>> m_hitTable;
    int32_t m_maximumHP;
    int32_t m_currentHP;
    Vector3 m_position;
    Vector3 m_rotation;
    Category m_category;
    uuids::uuid m_uuid;
};
}