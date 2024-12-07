#include <Common/Graphics/Node.hpp>
#include <Scenes/Game/System/Entities/BasicEntity.hpp>
#include <Scenes/Game/System/Field.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<BasicEntity> BasicEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<BasicEntity>(new BasicEntity(node));
}
BasicEntity::~BasicEntity() { }

void BasicEntity::update(Field& field)
{
    markAsDirtyAABB();
    rehashAABB();
    m_onGround = false;

    float dt = Time::deltaTime();
    float ep = Mathf::Epsilon;
    float threshould = 0.001f;
    Vector3 size = m_aabb.max - m_aabb.min;
    Vector3 delta = m_velocity * dt;
    Vector3 oldPos = getPosition();
    Vector3 newPos = oldPos + delta;

    // XŽ²•ûŒü
    {
        std::vector<IntVector3> fuzzyHits;
        Vector3 offset = delta * Vector3({ 1, 0, 0 });
        hitTilesFuzzy(field, offset, fuzzyHits);

        if (fuzzyHits.size() > 0) {
            if (m_velocity.x() > 0.0f) {
                float maxLen = 0.0f;
                std::vector<NodeHit> strictHits;
                hitTilesStrict(field, m_node, offset, fuzzyHits, strictHits);

                for (const auto& hit : strictHits) {
                    Geom::Plane plane;
                    plane.center = (Vector3)hit.tile * Field::k_tileSize;
                    plane.center += Vector3({ -0.5f, 0, 0 }) * Field::k_tileSize;
                    plane.normal = Vector3({ -1, 0, 0 });

                    Geom::OBB obb = hit.node->getOBB();
                    obb.center += offset;

                    float len = 0.0f;
                    if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                        if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                            maxLen = len;
                        }
                    }
                }
                if (maxLen != 0.0f) {
                    newPos.x() -= maxLen + threshould;
                    m_velocity.x() = 0.0f;
                }
            } else if (m_velocity.x() < 0.0f) {
                float maxLen = 0.0f;
                std::vector<NodeHit> strictHits;
                hitTilesStrict(field, m_node, offset, fuzzyHits, strictHits);

                for (const auto& hit : strictHits) {
                    Geom::Plane plane;
                    plane.center = (Vector3)hit.tile * Field::k_tileSize;
                    plane.center += Vector3({ 0.5f, 0, 0 }) * Field::k_tileSize;
                    plane.normal = Vector3({ 1, 0, 0 });

                    Geom::OBB obb = hit.node->getOBB();
                    obb.center += offset;

                    float len = 0.0f;
                    if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                        if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                            maxLen = len;
                        }
                    }
                }
                if (maxLen != 0.0f) {
                    newPos.x() += maxLen + threshould;
                    m_velocity.x() = 0.0f;
                }
            }
        }
    }

    // YŽ²•ûŒü
    {
        std::vector<IntVector3> fuzzyHits;
        Vector3 offset = delta * Vector3({ 0, 1, 0 });
        hitTilesFuzzy(field, offset, fuzzyHits);

        if (fuzzyHits.size() > 0) {
            if (m_velocity.y() > 0.0f) {
            } else if (m_velocity.y() < 0.0f) {
                float maxLen = 0.0f;
                std::vector<NodeHit> strictHits;
                hitTilesStrict(field, m_node, offset, fuzzyHits, strictHits);

                for (const auto& hit : strictHits) {
                    Geom::Plane plane;
                    plane.center = (Vector3)hit.tile * Field::k_tileSize;
                    plane.center += Vector3({ 0, 0.5f, 0 }) * Field::k_tileSize;
                    plane.normal = Vector3({ 0, 1, 0 });

                    Geom::OBB obb = hit.node->getOBB();
                    obb.center += offset;

                    float len = 0.0f;
                    if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                        if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                            maxLen = len;
                        }
                    }
                }
                if (maxLen != 0.0f) {
                    newPos.y() += maxLen + threshould;
                    m_velocity.y() = 0.0f;
                    m_onGround = true;
                }
            }
        }
    }

    // ZŽ²•ûŒü
    {
        std::vector<IntVector3> fuzzyHits;
        Vector3 offset = delta * Vector3({ 0, 0, 1 });
        hitTilesFuzzy(field, offset, fuzzyHits);

        if (fuzzyHits.size() > 0) {
            if (m_velocity.z() > 0.0f) {
                float maxLen = 0.0f;
                std::vector<NodeHit> strictHits;
                hitTilesStrict(field, m_node, offset, fuzzyHits, strictHits);

                for (const auto& hit : strictHits) {
                    Geom::Plane plane;
                    plane.center = (Vector3)hit.tile * Field::k_tileSize;
                    plane.center += Vector3({ 0, 0, -0.5f }) * Field::k_tileSize;
                    plane.normal = Vector3({ 0, 0, -1 });

                    Geom::OBB obb = hit.node->getOBB();
                    obb.center += offset;

                    float len = 0.0f;
                    if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                        if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                            maxLen = len;
                        }
                    }
                }
                if (maxLen != 0.0f) {
                    newPos.z() -= maxLen + threshould;
                    m_velocity.z() = 0.0f;
                }
            } else if (m_velocity.z() < 0.0f) {
                float maxLen = 0.0f;
                std::vector<NodeHit> strictHits;
                hitTilesStrict(field, m_node, offset, fuzzyHits, strictHits);

                for (const auto& hit : strictHits) {
                    Geom::Plane plane;
                    plane.center = (Vector3)hit.tile * Field::k_tileSize;
                    plane.center += Vector3({ 0, 0, 0.5f }) * Field::k_tileSize;
                    plane.normal = Vector3({ 0, 0, 1 });

                    Geom::OBB obb = hit.node->getOBB();
                    obb.center += offset;

                    float len = 0.0f;
                    if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                        if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                            maxLen = len;
                        }
                    }
                }
                if (maxLen != 0.0f) {
                    newPos.z() += maxLen + threshould;
                    m_velocity.z() = 0.0f;
                }
            }
        }
    }

    setPosition(newPos);

    if (m_receiveGravity) {
        m_velocity.y() -= Field::k_gravity * dt;
    }

    m_node->setLocalPosition(getPosition());

    Vector3 rot = m_torque * dt;
    Vector3 oldRot = getRotation();
    Vector3 newRot = oldRot + rot;

    newRot = Vector3({ Mathf::normalizeDegree(newRot.x()), Mathf::normalizeDegree(newRot.y()), Mathf::normalizeDegree(newRot.z()) });

    setRotation(newRot);
    m_node->setLocalRotation(newRot);
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

void BasicEntity::setTorque(const Vector3& torque) { m_torque = torque; }
Vector3 BasicEntity::getTorque() const { return m_torque; }

void BasicEntity::setReceiveGravity(bool receiveGravity) { m_receiveGravity = receiveGravity; }
bool BasicEntity::isReceiveGravity() const { return m_receiveGravity; }

bool BasicEntity::isOnGround() const { return m_onGround; }
// protected
BasicEntity::BasicEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : m_node(node)
    , m_aabb()
    , m_dirtyAABB(true)
    , m_velocity()
    , m_torque()
    , m_receiveGravity(true)
{
}
void BasicEntity::onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z) { }
void BasicEntity::onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z) { }
void BasicEntity::onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z) { }
// private
void BasicEntity::rehashAABB(const std::shared_ptr<Common::Graphics::Node>& node, Geom::AABB& dst)
{
    std::array<Vector3, 8> edges = node->getEdges();

    for (const auto& edge : edges) {
        float x = edge.x();
        float y = edge.y();
        float z = edge.z();

        if (x < dst.min.x()) {
            dst.min.x() = x;
        }
        if (y < dst.min.y()) {
            dst.min.y() = y;
        }
        if (z < dst.min.z()) {
            dst.min.z() = z;
        }
        if (x > dst.max.x()) {
            dst.max.x() = x;
        }
        if (y > dst.max.y()) {
            dst.max.y() = y;
        }
        if (z > dst.max.z()) {
            dst.max.z() = z;
        }
    }

    for (int32_t i = 0; i < node->getChildrenCount(); i++) {
        rehashAABB(node->getChildAt(i), dst);
    }
}

void BasicEntity::hitTilesFuzzy(Field& field, const Vector3& offset, std::vector<IntVector3>& hits)
{
    Vector3 center = m_aabb.min + ((m_aabb.max - m_aabb.min) / 2.0f);
    Vector3 size = (m_aabb.max - m_aabb.min);
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
                    if (block != 0) {
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
                if (block != 0) {
                    hits.emplace_back(IntVector3({ ix, iy, iz }));
                }
            }
        }
    }

    center += (dir * Field::k_tileSize);
    distance += Field::k_tileSize;
}

void BasicEntity::hitTilesStrict(Field& field, const std::shared_ptr<Common::Graphics::Node>& node, const Vector3& offset, std::vector<IntVector3>& checkTiles, std::vector<NodeHit>& hits)
{
    Geom::OBB nodeOBB = node->getOBB();
    nodeOBB.center += offset;

    for (const auto& checkTile : checkTiles) {
        if (!field.hasBlockAt(checkTile.x(), checkTile.y(), checkTile.z())) {
            continue;
        }

        Geom::OBB checkTileOBB;
        checkTileOBB.center = (Vector3)checkTile * Field::k_tileSize;
        checkTileOBB.axes = std::array<Vector3, 3> {
            Vector3({ 1, 0, 0 }),
            Vector3({ 0, 1, 0 }),
            Vector3({ 0, 0, 1 }),
        };
        checkTileOBB.size = Vector3({ 1, 1, 1 }) * Field::k_tileSize;

        bool isHit = Geom::Collision::testOBBAndOBB(nodeOBB, checkTileOBB);
        if (isHit) {
            NodeHit hit;
            hit.node = node;
            hit.tile = checkTile;
            hits.emplace_back(hit);
        }
    }
    for (int32_t i = 0; i < node->getChildrenCount(); i++) {
        auto c = node->getChildAt(i);
        hitTilesStrict(field, c, offset, checkTiles, hits);
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