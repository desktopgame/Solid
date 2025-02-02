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

static void physicsTest()
{
    auto normalTex = Texture::create("./assets/Textures/tileNormal2.png");
    auto borderTex = Texture::create("./assets/Textures/tileBorder.png");
    auto field = std::make_shared<System::Field>(normalTex, borderTex);
    field->generate();

    System::ChunkGenerator::Room room = field->getRoomAt(0);
    Random rand;
    for (int32_t i = 0; i < 10; i++) {
        int32_t halfX = (System::Chunk::k_roomSizeX / 2) - 3;
        int32_t halfZ = (System::Chunk::k_roomSizeZ / 2) - 3;
        int32_t tileOffsetX = rand.range(-halfX, halfX);
        int32_t tileOffsetZ = rand.range(-halfZ, halfZ);

        float enemyPosX = (room.center.x() + 0) * System::Chunk::k_tileSize;
        float enemyPosZ = (room.center.z() + 0) * System::Chunk::k_tileSize;

        auto enemy = System::Entities::SlimeEntity::create();
        enemy->setPosition(Vector3({ enemyPosX, 20, enemyPosZ }));
        field->spwan(enemy);
    }
    auto player = System::Entities::PlayerEntity::create(
        Common::Graphics::Node::deserialize("./assets/Models/Player.json"));
    player->setPosition(Vector3({ 999, 999, 999 }));
    player->setReceiveGravity(false);
    field->setPlayer(player);

    float elapsed = 0.0f;
    float deltaTime = 0.0288699996f;

    Geom::AABB fieldAABB;
    fieldAABB.min = (Vector3({ 0, 0, 0 }) * System::Chunk::k_tileSize) - (Vector3({ 0.5f, 0.5f, 0.5f }) * System::Chunk::k_tileSize);
    fieldAABB.max = (Vector3({ System::Chunk::k_chunkSizeX, System::Chunk::k_chunkSizeY, System::Chunk::k_chunkSizeZ }) * System::Chunk::k_tileSize) + (Vector3({ 0.5f, 0.5f, 0.5f }) * System::Chunk::k_tileSize);

    // 長すぎるとCIがタイムアウトするのでほどほどに
    const float duration = (60.0f * 10.0f * 1.0f);

    while (elapsed < duration) {
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

void runAllTests()
{
    physicsTest();
}
#endif