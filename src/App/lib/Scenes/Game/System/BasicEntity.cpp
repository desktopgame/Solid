#include <Common/Graphics/Node.hpp>
#include <Scenes/Game/System/BasicEntity.hpp>
#include <Scenes/Game/System/Field.hpp>

namespace App::Scenes::Game::System {
// public
std::shared_ptr<BasicEntity> BasicEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<BasicEntity>(new BasicEntity(node));
}
BasicEntity::~BasicEntity() { }

void BasicEntity::update(Field& field)
{
    rehashAABB();

    float dt = 0.01f;
    float ep = Mathf::Epsilon;
    Vector3 size = m_aabb.max - m_aabb.min;
    Vector3 delta = m_velocity * dt;
    Vector3 oldPos = getPosition();
    Vector3 newPos = oldPos + delta;

    {
        std::vector<IntVector3> hits;
        Vector3 offset = delta * Vector3({ 1, 0, 0 });
        Vector3 to = oldPos + offset;
        hitTiles(field, m_node, offset, hits, false);

        if (hits.size() > 0) {
            if (m_velocity.x() > 0.0f) {
                float minX = 9999.0f;
                bool hit = false;
                for (const auto& tile : hits) {
                    float baseX = (tile.x() * Field::k_tileSize);
                    baseX -= (Field::k_tileSize / 2.0f);
                    if (baseX <= (to.x() - (size.x() / 2.0f))) {
                        continue;
                    }
                    if (Mathf::abs((to.x() + (size.x() / 2.0f)) - ((tile.x() * Field::k_tileSize) - (Field::k_tileSize / 2.0f))) >= (Field::k_tileSize - 0.1f)) {
                        continue;
                    }
                    if (baseX < minX) {
                        minX = baseX;
                        hit = true;
                    }
                }
                if (hit) {
                    minX -= (size.x() / 2.0f) + ep;
                    newPos.x() = minX;
                    m_velocity.x() = 0.0f;
                }
            } else if (m_velocity.x() < 0.0f) {
                float maxX = -9999.0f;
                bool hit = false;
                for (const auto& tile : hits) {
                    float baseX = (tile.x() * Field::k_tileSize);
                    baseX += (Field::k_tileSize / 2.0f);
                    if (baseX >= (to.x() + (size.x() / 2.0f))) {
                        continue;
                    }
                    if (Mathf::abs((to.x() - (size.x() / 2.0f)) - ((tile.x() * Field::k_tileSize) + (Field::k_tileSize / 2.0f))) >= (Field::k_tileSize - 0.1f)) {
                        continue;
                    }
                    if (baseX > maxX) {
                        maxX = baseX;
                        hit = true;
                    }
                }
                if (hit) {
                    maxX += (size.x() / 2.0f) + ep;
                    newPos.x() = maxX;
                    m_velocity.x() = 0.0f;
                }
            }
        }
    }

    {
        std::vector<IntVector3> hits;
        Vector3 offset = delta * Vector3({ 0, 1, 0 });
        Vector3 to = oldPos + offset;
        hitTiles(field, m_node, offset, hits, false);

        if (hits.size() > 0) {
            if (m_velocity.y() > 0.0f) {
                float minY = 9999.0f;
                bool hit = false;
                for (const auto& tile : hits) {
                    float baseY = (tile.y() * Field::k_tileSize);
                    baseY -= (Field::k_tileSize / 2.0f);
                    if (baseY <= (to.y() - (size.y() / 2.0f))) {
                        continue;
                    }
                    if (Mathf::abs((to.y() + (size.y() / 2.0f)) - ((tile.y() * Field::k_tileSize) - (Field::k_tileSize / 2.0f))) >= (Field::k_tileSize - 0.1f)) {
                        continue;
                    }
                    if (baseY < minY) {
                        minY = baseY;
                        hit = true;
                    }
                }
                if (hit) {
                    minY -= (size.y() / 2.0f) + ep;
                    newPos.y() = minY;
                    m_velocity.y() = 0.0f;
                }
            } else if (m_velocity.y() < 0.0f) {
                float maxY = -9999.0f;
                bool hit = false;
                for (const auto& tile : hits) {
                    float baseY = (tile.y() * Field::k_tileSize);
                    baseY += (Field::k_tileSize / 2.0f);
                    if (baseY >= (to.y() + (size.y() / 2.0f))) {
                        continue;
                    }
                    if (Mathf::abs((to.y() - (size.y() / 2.0f)) - ((tile.y() * Field::k_tileSize) + (Field::k_tileSize / 2.0f))) >= (Field::k_tileSize - 0.1f)) {
                        continue;
                    }
                    if (baseY > maxY) {
                        maxY = baseY;
                        hit = true;
                    }
                }
                if (hit) {
                    maxY += (size.y() / 2.0f) + ep;
                    newPos.y() = maxY;
                    m_velocity.y() = 0.0f;
                }
            }
        }
    }

    {
        std::vector<IntVector3> hits;
        Vector3 offset = delta * Vector3({ 0, 0, 1 });
        Vector3 to = oldPos + offset;
        hitTiles(field, m_node, offset, hits, false);

        if (hits.size() > 0) {
            if (m_velocity.z() > 0.0f) {
                float minZ = 9999.0f;
                bool hit = false;
                for (const auto& tile : hits) {
                    float baseZ = (tile.z() * Field::k_tileSize);
                    baseZ -= (Field::k_tileSize / 2.0f);
                    if (baseZ <= (to.z() - (size.z() / 2.0f))) {
                        continue;
                    }
                    if (Mathf::abs((to.z() + (size.z() / 2.0f)) - ((tile.z() * Field::k_tileSize) - (Field::k_tileSize / 2.0f))) >= (Field::k_tileSize - 0.1f)) {
                        continue;
                    }
                    if (baseZ < minZ) {
                        minZ = baseZ;
                        hit = true;
                    }
                }
                if (hit) {
                    minZ -= (size.z() / 2.0f) + ep;
                    newPos.z() = minZ;
                    m_velocity.z() = 0.0f;
                }
            } else if (m_velocity.z() < 0.0f) {
                float maxZ = -9999.0f;
                bool hit = false;
                for (const auto& tile : hits) {
                    float baseZ = (tile.z() * Field::k_tileSize);
                    baseZ += (Field::k_tileSize / 2.0f);
                    if (baseZ >= (to.z() + (size.z() / 2.0f))) {
                        continue;
                    }
                    if (Mathf::abs((to.z() - (size.z() / 2.0f)) - ((tile.z() * Field::k_tileSize) + (Field::k_tileSize / 2.0f))) >= (Field::k_tileSize - 0.1f)) {
                        continue;
                    }
                    if (baseZ > maxZ) {
                        maxZ = baseZ;
                        hit = true;
                    }
                }
                if (hit) {
                    maxZ += (size.z() / 2.0f) + ep;
                    newPos.z() = maxZ;
                    m_velocity.z() = 0.0f;
                }
            }
        }
    }

    setPosition(newPos);
    m_velocity.y() -= Field::k_gravity * dt;

    m_node->setPosition(getPosition());
}
void BasicEntity::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    m_node->draw(renderer);
}
void BasicEntity::draw2D(const std::shared_ptr<Renderer>& renderer) { }

void BasicEntity::markAsDirtyAABB() { m_dirtyAABB = true; }
void BasicEntity::rehashAABB()
{
    if (m_dirtyAABB) {
        m_aabb.max = Vector3({ -9999, -9999, -9999 });
        m_aabb.min = Vector3({ 9999, 9999, 9999 });
        rehashAABB(m_node, m_aabb);
        m_dirtyAABB = false;
    }
}
Geom::AABB BasicEntity::getAABB() const { return m_aabb; }

void BasicEntity::setVelocity(const Vector3& velocity) { m_velocity = velocity; }
Vector3 BasicEntity::getVelocity() const { return m_velocity; }

void BasicEntity::setReceiveGravity(bool receiveGravity) { m_receiveGravity = receiveGravity; }
bool BasicEntity::isReceiveGravity() const { return m_receiveGravity; }
// protected
BasicEntity::BasicEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : m_node(node)
    , m_aabb()
    , m_dirtyAABB(true)
    , m_velocity()
    , m_receiveGravity(true)
{
}
// private
void BasicEntity::rehashAABB(const std::shared_ptr<Common::Graphics::Node>& node, Geom::AABB& dst)
{
    Vector3 center = node->getPosition();
    Vector3 size = node->getSize();

    float minX = center.x() - (size.x() / 2.0f);
    float maxX = center.x() + (size.x() / 2.0f);
    float minY = center.y() - (size.y() / 2.0f);
    float maxY = center.y() + (size.y() / 2.0f);
    float minZ = center.z() - (size.z() / 2.0f);
    float maxZ = center.z() + (size.z() / 2.0f);

    if (minX < dst.min.x()) {
        dst.min.x() = minX;
    }
    if (maxX > dst.max.x()) {
        dst.max.x() = maxX;
    }
    if (minY < dst.min.y()) {
        dst.min.y() = minY;
    }
    if (maxY > dst.max.y()) {
        dst.max.y() = maxY;
    }
    if (minZ < dst.min.z()) {
        dst.min.z() = minZ;
    }
    if (maxZ > dst.max.z()) {
        dst.max.z() = maxZ;
    }

    for (int32_t i = 0; i < node->getChildrenCount(); i++) {
        rehashAABB(node->getChildAt(i), dst);
    }
}

void BasicEntity::hitTiles(Field& field, const std::shared_ptr<Common::Graphics::Node>& node, const Vector3& offset, std::vector<IntVector3>& hits, bool always)
{
    Vector3 center = node->getAbsolutePosition();
    Vector3 size = node->getSize();
    Vector3 end = center + offset;
    Vector3 dir = Vector3::normalized(end - center);

    float distance = 0.0f;
    float limit = Vector3::distance(center, end);
    while (distance < limit) {
        float minX = alignTile(center.x() - (size.x() / 2.0f), Field::k_tileSize);
        float maxX = alignTile(center.x() + (size.x() / 2.0f), Field::k_tileSize);
        float minY = alignTile(center.y() - (size.y() / 2.0f), Field::k_tileSize);
        float maxY = alignTile(center.y() + (size.y() / 2.0f), Field::k_tileSize);
        float minZ = alignTile(center.z() - (size.z() / 2.0f), Field::k_tileSize);
        float maxZ = alignTile(center.z() + (size.z() / 2.0f), Field::k_tileSize);

        for (float fx = minX; fx <= maxX; fx += Field::k_tileSize) {
            for (float fy = minY; fy <= maxY; fy += Field::k_tileSize) {
                for (float fz = minZ; fz <= maxZ; fz += Field::k_tileSize) {
                    int32_t ix = static_cast<int32_t>(fx / Field::k_tileSize);
                    int32_t iy = static_cast<int32_t>(fy / Field::k_tileSize);
                    int32_t iz = static_cast<int32_t>(fz / Field::k_tileSize);
                    if (!field.hasBlockAt(ix, iy, iz)) {
                        continue;
                    }

                    int32_t block = field.getBlockAt(ix, iy, iz);
                    if (always || block != 0) {
                        hits.emplace_back(IntVector3({ ix, iy, iz }));
                    }
                }
            }
        }

        center += (dir * Field::k_tileSize);
        distance += Field::k_tileSize;
    }

    center = end;

    float minX = alignTile(center.x() - (size.x() / 2.0f), Field::k_tileSize);
    float maxX = alignTile(center.x() + (size.x() / 2.0f), Field::k_tileSize);
    float minY = alignTile(center.y() - (size.y() / 2.0f), Field::k_tileSize);
    float maxY = alignTile(center.y() + (size.y() / 2.0f), Field::k_tileSize);
    float minZ = alignTile(center.z() - (size.z() / 2.0f), Field::k_tileSize);
    float maxZ = alignTile(center.z() + (size.z() / 2.0f), Field::k_tileSize);

    for (float fx = minX; fx <= maxX; fx += Field::k_tileSize) {
        for (float fy = minY; fy <= maxY; fy += Field::k_tileSize) {
            for (float fz = minZ; fz <= maxZ; fz += Field::k_tileSize) {
                int32_t ix = static_cast<int32_t>(fx / Field::k_tileSize);
                int32_t iy = static_cast<int32_t>(fy / Field::k_tileSize);
                int32_t iz = static_cast<int32_t>(fz / Field::k_tileSize);
                if (!field.hasBlockAt(ix, iy, iz)) {
                    continue;
                }

                int32_t block = field.getBlockAt(ix, iy, iz);
                if (always || block != 0) {
                    hits.emplace_back(IntVector3({ ix, iy, iz }));
                }
            }
        }
    }

    center += (dir * Field::k_tileSize);
    distance += Field::k_tileSize;

    for (int32_t i = 0; i < node->getChildrenCount(); i++) {
        auto c = node->getChildAt(i);
        hitTiles(field, c, offset, hits, always);
    }
}

float BasicEntity::alignTile(float a, float tileSize)
{
    float tileHalf = tileSize / 2.0f;
    float d = a / tileSize;
    if (d > 0.0f) {
        d = ::floorf(d);
    } else {
        d = ::ceilf(d);
    }
    float m = ::fmodf(a, tileSize);

    if (::fabs(m) < 0.000001f) {
        return a;
    }
    if (m > 0.0f) {
        if (m < tileHalf) {
            return d * tileSize;
        }
        return (d * tileSize) + tileSize;
    } else {
        if (::fabs(m) < tileHalf) {
            return d * tileSize;
        }
        return (d * tileSize) - tileSize;
    }
}
}