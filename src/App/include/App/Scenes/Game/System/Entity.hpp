#pragma once
#include <App/library.hpp>
#include <memory>
#include <stduuid/uuid.h>
#include <unordered_map>

namespace App::Scenes::Game::System {
class Field;
class Chunk;
class DamageSource;
/**
 * 物理演算によってブロックと衝突し、ブロック単位に限らず自由に移動できるオブジェクトをエンティティと呼称します。
 */
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

    /**
     * エンティティの状態を更新します。
     * @param chunk
     */
    virtual void update(const std::shared_ptr<Chunk>& chunk) = 0;

    /**
     * エンティティのデバッグUIを描画します。
     */
    virtual void onGui() = 0;

    /**
     * エンティティの3D要素を描画します。
     * @param renderer
     */
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) = 0;

    /**
     * エンティティの2D要素を描画します。
     * @param renderer
     */
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) = 0;

    /**
     * 衝突してきたエンティティを記録します。
     * @param entity
     */
    void addHitEntity(const std::shared_ptr<Entity>& entity);

    /**
     * 衝突してきたエンティティを削除します、。
     * @param entity
     */
    void removeHitEntity(const std::shared_ptr<Entity>& entity);

    /**
     * 衝突してきたエンティティなら true を返します。
     * @param entity
     * @return
     */
    bool isHitOnEntity(const std::shared_ptr<Entity>& entity) const;

    /**
     * エンティティと衝突すると呼ばれるコールバックです。
     * @param entity
     */
    virtual void onHitEnterEntity(const std::shared_ptr<Entity>& entity) = 0;

    /**
     * エンティティと衝突している間呼ばれるコールバックです。
     * @param entity
     */
    virtual void onHitStayEntity(const std::shared_ptr<Entity>& entity) = 0;

    /**
     * エンティティとの衝突が終了すると呼ばれるコールバックです。
     * @param entity
     */
    virtual void onHitExitEntity(const std::shared_ptr<Entity>& entity) = 0;

    /**
     * ダメージ処理を実行し、まだ生きているなら true を返します。
     * @param damageSource
     * @return
     */
    virtual bool damage(const std::shared_ptr<DamageSource>& damageSource);

    /**
     * エンティティのOBBを返します。
     * @return
     */
    virtual Geom::OBB getOBB() const;

    /**
     * エンティティのAABBを返します。
     * @return
     */
    virtual Geom::AABB getAABB() const;

    /**
     * エンティティの最大HPを返します。
     * @return
     */
    int32_t getMaximumHP() const;

    /**
     * エンティティの現在HPを返します。
     * @return
     */
    int32_t getCurrentHP() const;

    /**
     * 既に死亡しているなら true を返します。
     * @return
     */
    bool isDead() const;

    /**
     * エンティティの位置を設定します。
     * @param position
     */
    void setPosition(const Vector3& position);

    /**
     * エンティティの位置を返します。
     * @return
     */
    Vector3 getPosition() const;

    /**
     * エンティティの回転を設定します。
     * @param rotation
     */
    void setRotation(const Vector3& rotation);

    /**
     * エンティティの回転を返します。
     * @return
     */
    Vector3 getRotation() const;

    /**
     * エンティティのカテゴリーを返します。
     * @return
     */
    Category getCategory() const;

    /**
     * エンティティの固有IDを返します。
     * @return
     */
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