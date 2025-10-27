#pragma once
#include <memory>
#include <string>

namespace App::Scenes::Game::System::Entities {
class PlayerEntity;
}

namespace App::Scenes::Game::System {
class Field;
class Chunk;
/**
 * プレイヤーが装備可能なウェポンを表すクラスです。
 */
class Weapon {
public:
    /**
     * ウェポンの共通パラメーターです。
     */
    class Parameter {
    public:
        int32_t power;
        float fireRate;
        float coolRate;
        float increaseEnergy;
        float decreaseEnergy;
    };

    /**
     * ウェポンの操作方法です。
     */
    enum class InputMethod {
        None,
        OneShot,
        Hold,
    };

    explicit Weapon(const Parameter& parameter);
    virtual ~Weapon();

    /**
     * 攻撃を実行します。
     * @param chunk
     * @param player
     */
    virtual void execute(const std::shared_ptr<Chunk>& chunk, const std::shared_ptr<Entities::PlayerEntity>& player) = 0;

    /**
     * 操作方法を返します。
     * @return
     */
    virtual InputMethod getInputMethod() const = 0;

    /**
     * エネルギーの最大量を返します。
     * @return
     */
    virtual float getEnergyMax() const = 0;

    /**
     * ウェポンのアイコン情報を返します。
     * @return
     */
    virtual std::string getIconPath() const = 0;

    const Parameter parameter;
};
}