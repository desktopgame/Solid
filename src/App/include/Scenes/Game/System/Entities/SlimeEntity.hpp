#pragma once
#include <Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class SlimeEntity : public EnemyEntity {
public:
    static std::shared_ptr<SlimeEntity> create(const std::shared_ptr<Common::Graphics::Node>& node);
    virtual ~SlimeEntity();

protected:
    SlimeEntity(const std::shared_ptr<Common::Graphics::Node>& node);

private:
};
}