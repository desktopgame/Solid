#pragma once
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class EnemyEntity : public BasicEntity {
public:
    virtual ~EnemyEntity();
    virtual bool damage(const std::shared_ptr<DamageSource>& damageSource) override;

protected:
    EnemyEntity(const std::shared_ptr<Common::Graphics::Node>& node);
    void onDead(const std::shared_ptr<DamageSource>& damageSource) override;

private:
};
}