#pragma once
#include <Scenes/Game/System/Entities/BasicEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class PlayerEntity : public BasicEntity {
public:
    static std::shared_ptr<PlayerEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~PlayerEntity();

    virtual void idle(Field& field) override;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) override;

protected:
    PlayerEntity(const std::shared_ptr<Common::Graphics::Node>& node);

private:
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;
};
}