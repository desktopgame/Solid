#ifdef _TEST
#include <Common/Graphics/Node.hpp>
#include <Common/Graphics/NodeRegistry.hpp>
#include <Scenes/Game/System/Entities/BasicEntity.hpp>
#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <test.hpp>

using namespace App;
using namespace App::Scenes::Game;

void runAllTests()
{
    auto normalTex = Texture::create("./assets/tileNormal2.png");
    auto borderTex = Texture::create("./assets/tileBorder.png");
    auto field = std::make_shared<System::Field>(normalTex, borderTex);
    field->load("assets\\Stages\\stage_base.csv");

    for (int32_t i = 0; i < 10; i++) {
        auto enemy = System::Entities::SlimeEntity::create();
        enemy->setPosition(Vector3({ 90, 20, 90 }));
        field->spwan(enemy);
    }
    auto player = System::Entities::PlayerEntity::create(
        Common::Graphics::Node::deserialize("assets\\Models\\player.json"));
    player->setPosition(Vector3({ 80, 20, 80 }));
    field->setPlayer(player);

    float elapsed = 0.0f;
    float deltaTime = 0.0288699996f;

    while (elapsed < (60.0f * 60.0f)) {
        Lib::Utils::Time::s_deltaTime = deltaTime;
        field->update();
        elapsed += deltaTime;
    }
}
#endif