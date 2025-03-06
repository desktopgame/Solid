#include <App/Scenes/Game/System/Chunk.hpp>
#include <App/Scenes/Game/System/ChunkGenerator.hpp>
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entities/SlimeEntity.hpp>
#include <App/Scenes/Game/System/Entity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <algorithm>
#include <imgui.h>

namespace App::Scenes::Game::System {
// public
Chunk::Chunk(
    std::weak_ptr<Field> field,
    const IntVector2& gridPosition,
    const std::shared_ptr<Texture>& normalTexture,
    const std::shared_ptr<Texture>& borderTexture)
    : m_blocks()
    , m_entities()
    , m_generator()
    , m_field(field)
    , m_gridPosition(gridPosition)
    , m_normalTexture(normalTexture)
    , m_borderTexture(borderTexture)
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_tileTransform()
    , m_tilePallet()
    , m_dangerColor({ 1.0f, 0.0f, 0.0f, 1.0f })
    , m_safeColor({ 0.0f, 1.0f, 0.0f, 1.0f })
    , m_colorProgress()
    , m_scanLineTime()
    , m_colorLerpTime()
    , m_instanceBuffers()
    , m_indexLength()
    , m_instanceCount()
#if _DEBUG
    , m_debugDrawChunk(true)
#endif
{
}

void Chunk::generate()
{
    m_vertexBuffer = CpuBuffer::create();
    m_indexBuffer = CpuBuffer::create();

    std::vector<VertexNormalTexCoord3D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generatePlane(vertices, indices);
    m_indexLength = static_cast<int32_t>(indices.size());

    m_vertexBuffer->allocate(sizeof(VertexNormalTexCoord3D) * vertices.size());
    m_vertexBuffer->update(vertices.data());

    m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_indexBuffer->update(indices.data());

    if (!m_generator) {
        m_generator = std::make_shared<ChunkGenerator>();
        m_generator->generate();

        // TODO: 仮置きの敵生成処理
        Random rand;
        int32_t enemyCount = rand.range(3, 10);
        for (int32_t i = 0; i < enemyCount; i++) {
            int32_t tileX = rand.range(Chunk::k_chunkInsetMinX, Chunk::k_chunkInsetMaxX - 1);
            int32_t tileZ = rand.range(Chunk::k_chunkInsetMinZ, Chunk::k_chunkInsetMaxZ - 1);
            IntVector3 globalTilePos = Field::toGlobalBlockPosition(m_gridPosition, tileX, 0, tileZ);

            float enemyPosX = globalTilePos.x() * System::Chunk::k_tileSize;
            float enemyPosZ = globalTilePos.z() * System::Chunk::k_tileSize;
            auto enemy = System::Entities::SlimeEntity::create();
            enemy->setPosition(Vector3({ enemyPosX, 10, enemyPosZ }));
            spwan(enemy);
        }
    }
    const std::vector<Vector4>& instances = m_generator->getTiles();

    std::vector<Vector4> gpuDatas = instances;
    for (auto& gpuData : gpuDatas) {
        int32_t offsetX = (m_gridPosition.x() * Chunk::k_chunkSizeX);
        int32_t offsetZ = (m_gridPosition.y() * Chunk::k_chunkSizeZ);
        gpuData.x() += offsetX;
        gpuData.z() += offsetZ;
    }

    auto instBuf = CpuBuffer::create();
    instBuf->allocate(sizeof(Vector4) * gpuDatas.size());
    instBuf->update(gpuDatas.data());
    m_instanceBuffers.emplace_back(instBuf);
    m_instanceCount = static_cast<int32_t>(gpuDatas.size());

    m_tileTransform.translateMatrixTable = k_translateMatrixTable;
    m_tileTransform.rotationMatrixTable = k_rotationMatrixTable;
    m_tileTransform.normalVectorTable = k_normalVectorTable;
    m_tilePallet.table = k_palletTable;

    for (int32_t x = 0; x < k_chunkSizeX; x++) {
        for (int32_t y = 0; y < k_chunkSizeY; y++) {
            for (int32_t z = 0; z < k_chunkSizeZ; z++) {
                m_blocks[toIndex(x, y, z)] = 0;
            }
        }
    }
    for (const auto& instance : instances) {
        int32_t x = static_cast<int32_t>(instance.x());
        int32_t y = static_cast<int32_t>(instance.y());
        int32_t z = static_cast<int32_t>(instance.z());
        m_blocks[toIndex(x, y, z)] = 1;
    }
}

void Chunk::update()
{
    auto self = shared_from_this();

    for (auto& entity : m_entities) {
        entity->update(self);
    }

    for (int32_t i = 0; i < static_cast<int32_t>(m_entities.size()); i++) {
        auto me = m_entities.at(i);
        Geom::AABB meAABB = me->getAABB();
        Geom::OBB meOBB = me->getOBB();

        for (int32_t j = 0; j < static_cast<int32_t>(m_entities.size()); j++) {
            if (i == j) {
                continue;
            }
            auto other = m_entities.at(j);
            Geom::AABB otherAABB = other->getAABB();

            bool hitEntity = false;
            if (Geom::Collision::testAABBAndAABB(meAABB, otherAABB)) {
                Geom::OBB otherOBB = other->getOBB();

                if (Geom::Collision::testOBBAndOBB(meOBB, otherOBB)) {
                    hitEntity = true;
                }
            }

            if (me->isHitOnEntity(other)) {
                if (hitEntity) {
                    me->onHitStayEntity(other);
                } else {
                    me->onHitExitEntity(other);
                    me->removeHitEntity(other);
                }
            } else {
                if (hitEntity) {
                    me->onHitEnterEntity(other);
                }
            }
        }
    }

    auto iter = std::remove_if(m_entities.begin(), m_entities.end(), [](const auto& e) -> bool {
        return e->isDead() || e->getPosition().y() < -100.0f || e->getPosition().y() > 50.0f;
    });
    m_entities.erase(iter, m_entities.end());

    if (!m_colorProgress) {
        int32_t enemyCount = countEntity(Entity::Category::Enemy);
        if (enemyCount == 0) {
            m_colorProgress = true;
        }
    }
    if (m_colorProgress) {
        if (m_scanLineTime < 0.5f) {
            m_scanLineTime = Mathf::min(0.5f, m_scanLineTime + Time::deltaTime());
        } else {
            m_colorLerpTime = Mathf::min(1.0f, m_colorLerpTime + Time::deltaTime());
        }
    }
}
void Chunk::onGui()
{
}
void Chunk::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    auto surface = Engine::getInstance()->getDevice()->getSurface();
    auto rc = RenderContext::get(Metadata::ProgramTable::TileInstance3D);
    auto ub = UniformPool::rent(Metadata::ProgramTable::TileInstance3D);
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = Matrix::transform(
        Matrix::translate(Vector3({ 0, 0, 0 })),
        Matrix(),
        Matrix::scale(Vector3({ k_tileSize, k_tileSize, k_tileSize })));
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    surface->uniformVS(ub, 0, &uCamera);
    surface->uniformVS(ub, 1, &m_tileTransform);
    surface->uniformVS(ub, 2, &m_tilePallet);

    Reflect::UVector3 uCameraPos;
    uCameraPos.value = Camera::getPosition();
    surface->uniformVS(ub, 3, &uCameraPos);

    Reflect::UVector4 uBorderColor;
    uBorderColor.value = Vector4::lerp(m_dangerColor, m_safeColor, m_colorLerpTime / 1.0f);
    surface->uniformVS(ub, 4, &uBorderColor);

    Reflect::UVector4 uFogColor;
    uFogColor.value = Vector4({ //
        Mathf::clamp(0.0f, 1.0f, Mathf::abs(static_cast<float>(m_gridPosition.x())) / 20.0f) * 0.5f,
        Mathf::clamp(0.0f, 1.0f, Mathf::abs(static_cast<float>(m_gridPosition.y())) / 20.0f) * 0.5f,
        1.0f,
        1.0f });
    surface->uniformVS(ub, 5, &uFogColor);

    surface->uniformPS(ub, 0, m_normalTexture);
    surface->uniformPS(ub, 1, m_borderTexture);

    Reflect::UFloat uScanLineY;
    uScanLineY.value = (m_scanLineTime / 0.5f) * (Chunk::k_chunkSizeY * 5.0f);
    surface->uniformPS(ub, 2, &uScanLineY);

#if _DEBUG
    if (m_debugDrawChunk)
#endif
    {
        Engine::getInstance()->getDevice()->getSurface()->render(
            rc,
            ub,
            m_vertexBuffer,
            m_indexBuffer,
            m_indexLength,
            m_instanceBuffers,
            m_instanceCount);
    }

    surface->beginBatch(RenderContext::get(Metadata::MeshColor3D));
    for (auto& entity : m_entities) {
        entity->draw3D(renderer);
    }
    surface->endBatch(RenderContext::get(Metadata::MeshColor3D));
}
void Chunk::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    for (auto& entity : m_entities) {
        entity->draw2D(renderer);
    }
}

std::shared_ptr<Field> Chunk::getField() const { return m_field.lock(); }

void Chunk::spwan(const std::shared_ptr<Entity>& entity) { m_entities.emplace_back(entity); }
std::shared_ptr<Entity> Chunk::getEntityAt(int32_t index) const { return m_entities.at(index); }
int32_t Chunk::getEntityCount() const { return static_cast<int32_t>(m_entities.size()); }

int32_t Chunk::countEntity(Entity::Category category) const
{
    int32_t count = 0;
    for (const auto& e : m_entities) {
        if (e->getCategory() == category) {
            count++;
        }
    }
    return count;
}
}