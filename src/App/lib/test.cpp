#ifdef _TEST
#include <Common/Graphics/Node.hpp>
#include <Common/Graphics/NodeRegistry.hpp>
#include <Scenes/Game/System/Entities/BasicEntity.hpp>
#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <cassert>
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
    player->setPosition(Vector3({ 999, 999, 999 }));
    player->setReceiveGravity(false);
    field->setPlayer(player);

    float elapsed = 0.0f;
    float deltaTime = 0.0288699996f;

    Geom::AABB fieldAABB;
    fieldAABB.min = (Vector3({ 0, 0, 0 }) * System::Field::k_tileSize) - (Vector3({ 0.5f, 0.5f, 0.5f }) * System::Field::k_tileSize);
    fieldAABB.max = (Vector3({ System::Field::k_fieldSizeX, System::Field::k_fieldSizeY, System::Field::k_fieldSizeZ }) * System::Field::k_tileSize) + (Vector3({ 0.5f, 0.5f, 0.5f }) * System::Field::k_tileSize);

    while (elapsed < (60.0f * 60.0f * 24.0f)) {
        Lib::Utils::Time::s_deltaTime = deltaTime;
        field->update();
        elapsed += deltaTime;

        for (int32_t i = 0; i < field->getEntityCount(); i++) {
            auto e = field->getEntityAt(i);
            if (e == player) {
                continue;
            }
            auto basicEntity = std::dynamic_pointer_cast<System::Entities::BasicEntity>(e);
            if (!basicEntity) {
                continue;
            }
            bool isHit = Geom::Collision::testAABBAndAABB(basicEntity->getAABB(), fieldAABB);
            if (!isHit) {
                std::cout << "Position: " << basicEntity->getPosition() << std::endl;
            }
            assert(isHit);

            bool highJump = basicEntity->getPosition().y() > 20.0f;
            if (highJump) {
                std::cout << "Position: " << basicEntity->getPosition() << std::endl;
            }
            assert(!highJump);
        }
    }
}
#endif