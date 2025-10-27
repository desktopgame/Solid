#pragma once
#include <memory>

namespace App::Scenes::Game::System {
class Entity;
/**
 * ダメージ情報を表すクラスです。
 */
class DamageSource {
public:
    /**
     * ダメージソースを生成します。
     * @param entity
     * @param power
     * @return
     */
    static std::shared_ptr<DamageSource> create(const std::shared_ptr<Entity>& entity, int32_t power);

    /**
     * 攻撃者のエンティティを返します。
     * @return
     */
    std::shared_ptr<Entity> getEntity() const;

    /**
     * 攻撃力を返します。
     * @return
     */
    int32_t getPower() const;

private:
    DamageSource();

    std::shared_ptr<Entity> m_entity;
    int32_t m_power;
};
}