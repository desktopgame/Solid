#pragma once
#include <Common/Graphics/Node.hpp>
#include <Scenes/Game/System/Entity.hpp>

namespace App::Scenes::Game::System {
class BasicEntity : public Entity {
public:
    static std::shared_ptr<BasicEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~BasicEntity();

    virtual void update(Field& field) override;
    virtual void draw3D(const std::shared_ptr<Renderer>& renderer) override;
    virtual void draw2D(const std::shared_ptr<Renderer>& renderer) override;

    void setVelocity(const Vector3& velocity);
    Vector3 getVelocity() const;

    void setReceiveGravity(bool receiveGravity);
    bool isReceiveGravity() const;

protected:
    BasicEntity(const std::shared_ptr<Common::Graphics::Node>& node);

private:
    std::shared_ptr<Common::Graphics::Node> m_node;
    Vector3 m_velocity;
    bool m_receiveGravity;
};
}