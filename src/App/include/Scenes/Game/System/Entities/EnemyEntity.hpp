#pragma once
#include <Scenes/Game/System/Entities/BasicEntity.hpp>

namespace App::Scenes::Game::System::Entities {
class EnemyEntity : public BasicEntity {
public:
    virtual ~EnemyEntity();

protected:
    EnemyEntity(const std::shared_ptr<Common::Graphics::Node>& node);

private:
};
}