#include <Common/Graphics/Node.hpp>
#include <Scenes/Game/System/Entities/BasicEntity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <iostream>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<BasicEntity> BasicEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<BasicEntity>(new BasicEntity(node));
}
BasicEntity::~BasicEntity() { }

void BasicEntity::idle(Field& field)
{
}
void BasicEntity::update(Field& field)
{
    float dt = Time::deltaTime();
    if (m_knockbackPlaying) {
        float t = m_knockbackElapsed / m_knockbackDuration;
        m_knockbackElapsed += dt;

        float power = 1.0f - t;
        m_velocity.x() += m_knockbackDirection.x() * (m_knockbackSpeed * power * dt);
        m_velocity.z() += m_knockbackDirection.z() * (m_knockbackSpeed * power * dt);

        if (t >= 1.0f) {
            m_knockbackPlaying = false;
            m_knockbackElapsed = 0.0f;
            m_velocity.x() = 0.0f;
            m_velocity.z() = 0.0f;
            idle(field);
        }
    } else {
        idle(field);
    }

    if (m_damagePlaying) {
        float t = m_damageElapsed / m_damageDuration;
        m_damageElapsed += dt;

        if (t >= 1.0f) {
            m_damagePlaying = false;
        }
    }

    m_node->validate();
    markAsDirtyAABB();
    rehashAABB();
    m_onGround = false;

    float threshould = 0.001f;
    Vector3 delta = m_velocity * dt;
    Vector3 oldPos = getPosition();
    Vector3 newPos = oldPos + delta;

    // X軸方向
    if (Mathf::abs(m_velocity.x()) > 0) {
        Vector3 offset = delta * Vector3({ 1, 0, 0 });
        m_fuzzyHitCache.clear();
        hitTilesFuzzy(field, offset, m_fuzzyHitCache);

        if (m_fuzzyHitCache.size() > 0) {
            if (m_velocity.x() > 0.0f) {
                Vector3 totalOffset = offset;
                while (true) {
                    float maxLen = 0.0f;

                    m_fuzzyHitCache.clear();
                    hitTilesFuzzy(field, totalOffset, m_fuzzyHitCache);

                    m_strictHitCache.clear();
                    hitTilesStrict(field, m_node, totalOffset, m_fuzzyHitCache, m_strictHitCache);

                    IntVector3 hitTile;
                    for (const auto& hit : m_strictHitCache) {
                        Geom::Plane plane;
                        plane.center = (Vector3)hit.tile * Field::k_tileSize;
                        plane.center += Vector3({ -0.5f, 0, 0 }) * Field::k_tileSize;
                        plane.normal = Vector3({ -1, 0, 0 });

                        Geom::OBB obb = hit.node->getOBB();
                        obb.center += totalOffset;

                        float len = 0.0f;
                        if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                            if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                                maxLen = len;
                                hitTile = hit.tile;
                            }
                        }
                    }
                    if (maxLen != 0.0f) {
                        newPos.x() -= maxLen + threshould;
                        totalOffset.x() -= maxLen + threshould;
                        m_velocity.x() = 0.0f;
                        onCollisionWall(field, hitTile.x(), hitTile.y(), hitTile.z());
                    } else {
                        break;
                    }
                }
            } else if (m_velocity.x() < 0.0f) {
                Vector3 totalOffset = offset;
                while (true) {
                    float maxLen = 0.0f;

                    m_fuzzyHitCache.clear();
                    hitTilesFuzzy(field, totalOffset, m_fuzzyHitCache);

                    m_strictHitCache.clear();
                    hitTilesStrict(field, m_node, totalOffset, m_fuzzyHitCache, m_strictHitCache);

                    IntVector3 hitTile;
                    for (const auto& hit : m_strictHitCache) {
                        Geom::Plane plane;
                        plane.center = (Vector3)hit.tile * Field::k_tileSize;
                        plane.center += Vector3({ 0.5f, 0, 0 }) * Field::k_tileSize;
                        plane.normal = Vector3({ 1, 0, 0 });

                        Geom::OBB obb = hit.node->getOBB();
                        obb.center += totalOffset;

                        float len = 0.0f;
                        if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                            if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                                maxLen = len;
                                hitTile = hit.tile;
                            }
                        }
                    }
                    if (maxLen != 0.0f) {
                        newPos.x() += maxLen + threshould;
                        totalOffset.x() += maxLen + threshould;
                        m_velocity.x() = 0.0f;
                        onCollisionWall(field, hitTile.x(), hitTile.y(), hitTile.z());
                    } else {
                        break;
                    }
                }
            }
        }
    }

    // Y軸方向
    if (Mathf::abs(m_velocity.y()) > 0) {
        Vector3 offset = delta * Vector3({ 0, 1, 0 });

        m_fuzzyHitCache.clear();
        hitTilesFuzzy(field, offset, m_fuzzyHitCache);

        if (m_fuzzyHitCache.size() > 0) {
            if (m_velocity.y() > 0.0f) {
                Vector3 totalOffset = offset;
                while (true) {
                    float maxLen = 0.0f;

                    m_fuzzyHitCache.clear();
                    hitTilesFuzzy(field, totalOffset, m_fuzzyHitCache);

                    m_strictHitCache.clear();
                    hitTilesStrict(field, m_node, totalOffset, m_fuzzyHitCache, m_strictHitCache);

                    IntVector3 hitTile;
                    for (const auto& hit : m_strictHitCache) {
                        Geom::Plane plane;
                        plane.center = (Vector3)hit.tile * Field::k_tileSize;
                        plane.center += Vector3({ 0, -0.5f, 0 }) * Field::k_tileSize;
                        plane.normal = Vector3({ 0, -1, 0 });

                        Geom::OBB obb = hit.node->getOBB();
                        obb.center += totalOffset;

                        float len = 0.0f;
                        if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                            if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                                maxLen = len;
                                hitTile = hit.tile;
                            }
                        }
                    }
                    if (maxLen != 0.0f) {
                        newPos.y() -= maxLen + threshould;
                        totalOffset.y() -= maxLen + threshould;
                        m_velocity.y() = 0.0f;
                        onCollisionRoof(field, hitTile.x(), hitTile.y(), hitTile.z());
                    } else {
                        break;
                    }
                }
            } else if (m_velocity.y() < 0.0f) {
                Vector3 totalOffset = offset;
                while (true) {
                    float maxLen = 0.0f;

                    m_fuzzyHitCache.clear();
                    hitTilesFuzzy(field, totalOffset, m_fuzzyHitCache);

                    m_strictHitCache.clear();
                    hitTilesStrict(field, m_node, totalOffset, m_fuzzyHitCache, m_strictHitCache);

                    IntVector3 hitTile;
                    for (const auto& hit : m_strictHitCache) {
                        Geom::Plane plane;
                        plane.center = (Vector3)hit.tile * Field::k_tileSize;
                        plane.center += Vector3({ 0, 0.5f, 0 }) * Field::k_tileSize;
                        plane.normal = Vector3({ 0, 1, 0 });

                        Geom::OBB obb = hit.node->getOBB();
                        obb.center += totalOffset;

                        float len = 0.0f;
                        if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                            if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                                maxLen = len;
                                hitTile = hit.tile;
                            }
                        }
                    }
                    if (maxLen != 0.0f) {
                        newPos.y() += maxLen + threshould;
                        totalOffset.y() += maxLen + threshould;
                        m_velocity.y() = 0.0f;
                        m_onGround = true;
                        onCollisionFloor(field, hitTile.x(), hitTile.y(), hitTile.z());
                    } else {
                        break;
                    }
                }
            }
        }
    }

    // Z軸方向
    if (Mathf::abs(m_velocity.z()) > 0) {
        Vector3 offset = delta * Vector3({ 0, 0, 1 });
        m_fuzzyHitCache.clear();
        hitTilesFuzzy(field, offset, m_fuzzyHitCache);

        if (m_fuzzyHitCache.size() > 0) {
            if (m_velocity.z() > 0.0f) {
                Vector3 totalOffset = offset;
                while (true) {
                    float maxLen = 0.0f;

                    m_fuzzyHitCache.clear();
                    hitTilesFuzzy(field, totalOffset, m_fuzzyHitCache);

                    m_strictHitCache.clear();
                    hitTilesStrict(field, m_node, totalOffset, m_fuzzyHitCache, m_strictHitCache);

                    IntVector3 hitTile;
                    for (const auto& hit : m_strictHitCache) {
                        Geom::Plane plane;
                        plane.center = (Vector3)hit.tile * Field::k_tileSize;
                        plane.center += Vector3({ 0, 0, -0.5f }) * Field::k_tileSize;
                        plane.normal = Vector3({ 0, 0, -1 });

                        Geom::OBB obb = hit.node->getOBB();
                        obb.center += totalOffset;

                        float len = 0.0f;
                        if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                            if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                                maxLen = len;
                                hitTile = hit.tile;
                            }
                        }
                    }
                    if (maxLen != 0.0f) {
                        newPos.z() -= maxLen + threshould;
                        totalOffset.z() -= maxLen + threshould;
                        m_velocity.z() = 0.0f;
                        onCollisionWall(field, hitTile.x(), hitTile.y(), hitTile.z());
                    } else {
                        break;
                    }
                }
            } else if (m_velocity.z() < 0.0f) {
                Vector3 totalOffset = offset;
                while (true) {
                    float maxLen = 0.0f;

                    m_fuzzyHitCache.clear();
                    hitTilesFuzzy(field, totalOffset, m_fuzzyHitCache);

                    m_strictHitCache.clear();
                    hitTilesStrict(field, m_node, totalOffset, m_fuzzyHitCache, m_strictHitCache);

                    IntVector3 hitTile;
                    for (const auto& hit : m_strictHitCache) {
                        Geom::Plane plane;
                        plane.center = (Vector3)hit.tile * Field::k_tileSize;
                        plane.center += Vector3({ 0, 0, 0.5f }) * Field::k_tileSize;
                        plane.normal = Vector3({ 0, 0, 1 });

                        Geom::OBB obb = hit.node->getOBB();
                        obb.center += totalOffset;

                        float len = 0.0f;
                        if (Geom::Collision::testOBBAndPlane(obb, plane, len)) {
                            if (Mathf::abs(len) > Mathf::abs(maxLen)) {
                                maxLen = len;
                                hitTile = hit.tile;
                            }
                        }
                    }
                    if (maxLen != 0.0f) {
                        newPos.z() += maxLen + threshould;
                        totalOffset.z() += maxLen + threshould;
                        m_velocity.z() = 0.0f;
                        onCollisionWall(field, hitTile.x(), hitTile.y(), hitTile.z());
                    } else {
                        break;
                    }
                }
            }
        }
    }

    // めりこみのチェック
    Geom::AABB saveAABB = m_aabb;
    m_aabb.min += newPos - oldPos;
    m_aabb.max += newPos - oldPos;

    std::vector<IntVector3> tmpHits;
    hitTilesFuzzy(field, Vector3({ 0, 0, 0 }), tmpHits);

    if (!tmpHits.empty()) {
        newPos = oldPos;
        m_velocity = Vector3({ 0, 0, 0 });
    }
    m_aabb = saveAABB;

    setPosition(newPos);

    if (m_receiveGravity) {
        m_velocity.y() -= Field::k_gravity * dt;
    }

    Vector3 rot = m_torque * dt;
    Vector3 oldRot = getRotation();
    Vector3 newRot = oldRot + rot;

    newRot = Vector3({ Mathf::normalizeDegree(newRot.x()), Mathf::normalizeDegree(newRot.y()), Mathf::normalizeDegree(newRot.z()) });

    if (m_torque.length() > 0.0f) {
        m_node->validate();
        markAsDirtyAABB();
        rehashAABB();

        float low = 0.0f;
        float high = 1.0f;
        const int32_t maxIterations = 10;

        float best = 0.0f;
        for (int32_t i = 0; i < maxIterations; i++) {
            float mid = (low + high) * 0.5f;
            newRot = oldRot + (m_torque * dt * mid);
            newRot = Vector3({ Mathf::normalizeDegree(newRot.x()), Mathf::normalizeDegree(newRot.y()), Mathf::normalizeDegree(newRot.z()) });

            setRotation(newRot);
            m_node->validate();

            markAsDirtyAABB();
            rehashAABB();

            m_fuzzyHitCache.clear();
            hitTilesFuzzy(field, Vector3({ 0, 0, 0 }), m_fuzzyHitCache);

            if (!m_fuzzyHitCache.empty()) {
                high = mid;
            } else {
                low = mid;
                best = mid;
            }
        }

        if (best < 0.0001f) {
            best = 0.0f;
        }

        newRot = oldRot + (m_torque * dt * best);
        newRot = Vector3({ Mathf::normalizeDegree(newRot.x()), Mathf::normalizeDegree(newRot.y()), Mathf::normalizeDegree(newRot.z()) });
        setRotation(newRot);
        m_node->validate();

        markAsDirtyAABB();
        rehashAABB();

        if (best == 0.0f) {
            onRotationStop(field);
        }
    }
}
void BasicEntity::onGui() { }
void BasicEntity::draw3D(const std::shared_ptr<Renderer>& renderer)
{
    Vector3 blendColor = Vector3({ 1, 1, 1 });
    float blendRate = 0.0f;
    if (m_damagePlaying) {
        blendColor.x() = 1.0f;
        blendColor.y() = 0.0f;
        blendColor.z() = 0.0f;
        blendRate = m_damageElapsed / m_damageDuration;
    }
    m_node->draw(renderer, blendColor, blendRate);
}
void BasicEntity::draw2D(const std::shared_ptr<Renderer>& renderer) { }
void BasicEntity::onHitEnterEntity(const std::shared_ptr<Entity>& entity) { }
void BasicEntity::onHitStayEntity(const std::shared_ptr<Entity>& entity) { }
void BasicEntity::onHitExitEntity(const std::shared_ptr<Entity>& entity) { }

bool BasicEntity::damage(const std::shared_ptr<DamageSource>& damageSource)
{
    if (!m_damagePlaying) {
        m_damageElapsed = 0.0f;
        m_damageDuration = 0.25f;
        m_damagePlaying = true;
        return Entity::damage(damageSource);
    }
    return false;
}

void BasicEntity::knockback(const Vector3& direction, float speed, float duration)
{
    if (!m_knockbackPlaying) {
        m_knockbackDirection = direction;
        m_knockbackSpeed = speed;
        m_knockbackDuration = duration;
        m_knockbackPlaying = true;
    }
}

std::shared_ptr<Common::Graphics::Node> BasicEntity::getNode() const { return m_node; }
Geom::OBB BasicEntity::getOBB() const { return m_node->getOBB(); }

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
    , m_groundTile()
    , m_knockbackDirection()
    , m_knockbackSpeed()
    , m_knockbackElapsed()
    , m_knockbackDuration()
    , m_knockbackPlaying()
    , m_damageElapsed()
    , m_damageDuration()
    , m_damagePlaying()
    , m_aabb()
    , m_dirtyAABB(true)
    , m_velocity()
    , m_torque()
    , m_receiveGravity(true)
{
}

void BasicEntity::onPositionChanged(const Vector3& position) { m_node->setLocalPosition(position); }
void BasicEntity::onRotationChanged(const Vector3& rotation) { m_node->setLocalRotation(rotation); }
void BasicEntity::onCollisionWall(Field& field, int32_t x, int32_t y, int32_t z) { }
void BasicEntity::onCollisionRoof(Field& field, int32_t x, int32_t y, int32_t z) { }
void BasicEntity::onCollisionFloor(Field& field, int32_t x, int32_t y, int32_t z) { }
void BasicEntity::onRotationStop(Field& field) { }
// private
void BasicEntity::rehashAABB(const std::shared_ptr<Common::Graphics::Node>& node, Geom::AABB& dst)
{
    node->validate();
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

}