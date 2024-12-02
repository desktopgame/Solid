#pragma once
#include <Scenes/Game/System/Entities/EnemyEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class SlimeEntity : public EnemyEntity {
public:
    static std::shared_ptr<SlimeEntity> create();
    virtual ~SlimeEntity();

protected:
    SlimeEntity();

private:
};
}