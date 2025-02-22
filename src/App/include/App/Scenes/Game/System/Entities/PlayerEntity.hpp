#pragma once
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>
#include <App/Scenes/Game/System/Weapon.hpp>

namespace App::Scenes::Game::System::Entities {
class PlayerEntity : public BasicEntity {
public:
    static std::shared_ptr<PlayerEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~PlayerEntity();

    virtual void idle(const std::shared_ptr<Chunk>& chunk) override;
    virtual void update(const std::shared_ptr<Chunk>& chunk) override;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) override;

    void setMainWeapon(const std::shared_ptr<Weapon>& mainWeapon);
    std::shared_ptr<Weapon> getMainWeapon() const;

    void setSubWeapon(const std::shared_ptr<Weapon>& subWeapon);
    std::shared_ptr<Weapon> getSubWeapon() const;

protected:
    PlayerEntity(const std::shared_ptr<Common::Graphics::Node>& node);

private:
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;

    std::shared_ptr<System::Weapon> m_mainWeapon;
    float m_mainWeaponEnergy;
    float m_mainWeaponRemain;

    std::shared_ptr<System::Weapon> m_subWeapon;
    float m_subWeaponEnergy;
    float m_subWeaponRemain;
};
}