#ifdef _TEST
#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Scenes/Game/System/Entities/BasicEntity.hpp>
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <App/test.hpp>
#include <cassert>


using namespace App;
using namespace App::Scenes::Game;

static void physicsTest()
{
    auto normalTex = Texture::create("./assets/Textures/tileNormal2.png");
    auto borderTex = Texture::create("./assets/Textures/tileBorder.png");
    auto field = std::make_shared<System::Field>(normalTex, borderTex);
    field->generate();

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

        for (int32_t i = 0; i < field->getCurrentChunk()->getEntityCount(); i++) {
            auto e = field->getCurrentChunk()->getEntityAt(i);
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