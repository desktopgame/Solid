#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <algorithm>
#include <imgui.h>

namespace App::Scenes::Game::System {
// public
Field::Field(
    const std::shared_ptr<Texture>& normalTexture,
    const std::shared_ptr<Texture>& borderTexture)
    : m_blocks()
    , m_player()
    , m_entities()
    , m_normalTexture(normalTexture)
    , m_borderTexture(borderTexture)
    , m_vertexBuffer()
    , m_indexBuffer()
    , m_tileTransform()
    , m_tilePallet()
    , m_instanceBuffers()
    , m_indexLength()
    , m_instanceCount()
#if _DEBUG
    , m_debugDrawField(true)
#endif
{
}

void Field::load(const std::string& file)
{
    m_vertexBuffer = Buffer::create();
    m_indexBuffer = Buffer::create();

    std::vector<VertexNormalTexCoord3D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generatePlane(vertices, indices);
    m_indexLength = static_cast<int32_t>(indices.size());

    m_vertexBuffer->allocate(sizeof(VertexNormalTexCoord3D) * vertices.size());
    m_vertexBuffer->update(vertices.data());

    m_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    m_indexBuffer->update(indices.data());

    auto instBuf = Buffer::create();
    std::vector<Vector4> instances;
    IO::deserializeTile(file, instances, 1.0f);
    instBuf->allocate(sizeof(Vector4) * instances.size());
    instBuf->update(instances.data());
    m_instanceBuffers.emplace_back(instBuf);
    m_instanceCount = static_cast<int32_t>(instances.size());

    m_tileTransform.translateMatrixTable = k_translateMatrixTable;
    m_tileTransform.rotationMatrixTable = k_rotationMatrixTable;
    m_tileTransform.normalVectorTable = k_normalVectorTable;
    m_tilePallet.table = k_palletTable;

    for (int32_t x = 0; x < k_fieldSizeX; x++) {
        for (int32_t y = 0; y < k_fieldSizeY; y++) {
            for (int32_t z = 0; z < k_fieldSizeZ; z++) {
                m_blocks[x][y][z] = 0;
            }
        }
    }
    for (const auto& instance : instances) {
        int32_t x = static_cast<int32_t>(instance.x());
        int32_t y = static_cast<int32_t>(instance.y());
        int32_t z = static_cast<int32_t>(instance.z());
        m_blocks[x][y][z] = 1;
    }
}

void Field::update()
{
    Field& self = *this;

    m_player->update(self);
    for (auto& entity : m_entities) {
        entity->update(self);
    }

    auto iter = std::remove_if(m_entities.begin(), m_entities.end(), [](const auto& e) -> bool {
        return e->isDead();
    });
    m_entities.erase(iter, m_entities.end());
}
void Field::onGui()
{
#if _DEBUG
    ImGui::Begin("Field");
    ImGui::Checkbox("Draw", &m_debugDrawField);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (m_player) {
        if (ImGui::TreeNodeEx("Player", flags)) {
            m_player->onGui();
            ImGui::TreePop();
        }
    }

    int32_t index = 0;
    for (const auto& entity : m_entities) {
        char buf[16];
        ::memset(buf, '\0', 16);
        ::sprintf(buf, "Entity[%d]", index);
        index += 1;

        if (ImGui::TreeNodeEx(buf, flags)) {
            entity->onGui();
            ImGui::TreePop();
        }
    }
    ImGui::End();
#endif
}
void Field::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    auto rc = RenderContext::get(Metadata::ProgramTable::TileInstance3D);
    auto ub = UniformPool::rent(Metadata::ProgramTable::TileInstance3D);
    Reflect::UCamera uCamera;
    uCamera.modelMatrix = Matrix::transform(
        Matrix::translate(Vector3({ 0, 0, 0 })),
        Matrix(),
        Matrix::scale(Vector3({ k_tileSize, k_tileSize, k_tileSize })));
    uCamera.viewMatrix = Camera::getLookAtMatrix();
    uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
    ub->setVS(0, &uCamera);
    ub->setVS(1, &m_tileTransform);
    ub->setVS(2, &m_tilePallet);

    Reflect::UVector3 uCameraPos;
    uCameraPos.value = Camera::getPosition();
    ub->setVS(3, &uCameraPos);

    ub->setPS(0, m_normalTexture);
    ub->setPS(1, m_borderTexture);
#if _DEBUG
    if (m_debugDrawField)
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
    m_player->draw3D(renderer);
    for (auto& entity : m_entities) {
        entity->draw3D(renderer);
    }
}
void Field::draw2D(const std::shared_ptr<Renderer>& renderer)
{
    m_player->draw2D(renderer);
    for (auto& entity : m_entities) {
        entity->draw2D(renderer);
    }
}

bool Field::hasBlockAt(int32_t x, int32_t y, int32_t z) const
{
    if (x >= k_fieldSizeX || x < 0) {
        return false;
    }
    if (y >= k_fieldSizeY || y < 0) {
        return false;
    }
    if (z >= k_fieldSizeZ || z < 0) {
        return false;
    }
    return true;
}

int32_t Field::getBlockAt(int32_t x, int32_t y, int32_t z) const
{
    if (!hasBlockAt(x, y, z)) {
        return 1;
    }
    return m_blocks[x][y][z];
}

void Field::setPlayer(const std::shared_ptr<Entities::PlayerEntity>& player) { m_player = player; }
std::shared_ptr<Entities::PlayerEntity> Field::getPlayer() const { return m_player; }

void Field::spwan(const std::shared_ptr<Entity>& entity) { m_entities.emplace_back(entity); }
std::shared_ptr<Entity> Field::getEntityAt(int32_t index) const { return m_entities.at(index); }
int32_t Field::getEntityCount() const { return static_cast<int32_t>(m_entities.size()); }

}