#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entities/ProjectileEntity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <App/Scenes/Game/System/Weapons/SingleOneShotWeapon.hpp>

namespace App::Scenes::Game::System::Entities {
// public
std::shared_ptr<PlayerEntity> PlayerEntity::create(const std::shared_ptr<Common::Graphics::Node>& node)
{
    return std::shared_ptr<PlayerEntity>(new PlayerEntity(node));
}
PlayerEntity::~PlayerEntity() { }

void PlayerEntity::idle(const std::shared_ptr<Chunk>& chunk)
{
    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    auto right = Quaternion::transform(rotation * Quaternion::angleAxis(90.0f, Vector3({ 0, 1, 0 })), Vector3({ 0, 0, 1 }));

    auto keyboard = InputSystem::getInstance()->getKeyboard();
    auto mouse = InputSystem::getInstance()->getMouse();

    Vector3 addVelocity;
    if (Cursor::isLocked()) {
        if (keyboard->isPressed(KeyCode::W)) {
            addVelocity += forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::S)) {
            addVelocity -= forward * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::D)) {
            addVelocity += right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        } else if (keyboard->isPressed(KeyCode::A)) {
            addVelocity -= right * Vector3({ 1, 0, 1 }) * m_cameraMoveSpeed;
        }

        bool isJump = false;
        if (keyboard->isPressed(KeyCode::Space)) {
            isJump = true;
        }

        if (isOnGround() && isJump) {
            setVelocity(Vector3({ 0, 30, 0 }));
        } else {
            if (addVelocity.length() > 0.0f) {
                setVelocity(getVelocity() * Vector3({ 0, 1, 0 }) + addVelocity);
            } else {
                setVelocity(getVelocity() * Vector3({ 0, 1, 0 }));
            }
        }

        auto mouseDelta = mouse->getDelta();
        m_cameraAngleX -= static_cast<float>(mouseDelta.y()) * m_cameraRotateSpeed;
        m_cameraAngleY += static_cast<float>(mouseDelta.x()) * m_cameraRotateSpeed;

        if (m_cameraAngleX <= -90.0f) {
            m_cameraAngleX = -89.0f;
        }
        if (m_cameraAngleX >= 90.0f) {
            m_cameraAngleX = 89.0f;
        }
    }

    if (mouse->isTrigger(Mouse::Button::Left)) {
        auto proj = ProjectileEntity::create(Common::Graphics::NodeRegistry::s_bulletNode->clone(), Entity::Category::PlayerTeam);
        proj->setOwner(shared_from_this());
        proj->setPosition(getPosition() + Vector3({ 0, 5, 0 }));
        proj->setRotation(Vector3({ m_cameraAngleX, Mathf::normalizeDegree(-m_cameraAngleY), 0.0f }));
        proj->setDirection(forward);
        proj->setSpeed(200);
        chunk->spwan(proj);
    }
}
void PlayerEntity::update(const std::shared_ptr<Chunk>& chunk)
{
    BasicEntity::update(chunk);

    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    Camera::position(getPosition());
    Camera::lookAt(getPosition() + forward);
    Camera::depthRange(0.1f, 1000.0f);

    fireMainWeapon(chunk);
    coolMainWeapon();

    fireSubWeapon(chunk);
    coolSubWeapon();
}
void PlayerEntity::draw3D(const std::shared_ptr<Renderer>& renderer) { }
void PlayerEntity::draw2D(const std::shared_ptr<Renderer>& renderer) { }

void PlayerEntity::setMainWeapon(const std::shared_ptr<Weapon>& mainWeapon)
{
    m_mainWeapon = mainWeapon;
    m_mainWeaponEnergy = mainWeapon->getEnergyMax();
}
std::shared_ptr<Weapon> PlayerEntity::getMainWeapon() const { return m_mainWeapon; }

void PlayerEntity::setSubWeapon(const std::shared_ptr<Weapon>& subWeapon)
{
    m_subWeapon = subWeapon;
    m_subWeaponEnergy = subWeapon->getEnergyMax();
}
std::shared_ptr<Weapon> PlayerEntity::getSubWeapon() const { return m_subWeapon; }

// protected
PlayerEntity::PlayerEntity(const std::shared_ptr<Common::Graphics::Node>& node)
    : BasicEntity(node)
    , m_cameraAngleX()
    , m_cameraAngleY()
    , m_cameraMoveSpeed(40.0f)
    , m_cameraRotateSpeed(0.8f)
    , m_mainWeapon()
    , m_mainWeaponEnergy()
    , m_mainWeaponFireRemain()
    , m_mainWeaponCoolRemain()
    , m_subWeapon()
    , m_subWeaponEnergy()
    , m_subWeaponFireRemain()
    , m_subWeaponCoolRemain()
{
    m_category = Entity::Category::Player;
}

void PlayerEntity::fireMainWeapon(const std::shared_ptr<Chunk>& chunk)
{
    if (!m_mainWeapon) {
        return;
    }
    if (m_mainWeaponFireRemain > 0.0f) {
        m_mainWeaponFireRemain = Mathf::max(0.0f, m_mainWeaponFireRemain - Time::deltaTime());
        return;
    }
    auto mouse = InputSystem::getInstance()->getMouse();
    auto inputMethod = m_mainWeapon->getInputMethod();
    bool inputOk = false;
    switch (inputMethod) {
    case Weapon::InputMethod::None:
        break;
    case Weapon::InputMethod::LeftOneShot:
        inputOk = mouse->getState(Mouse::Button::Left) == ButtonState::Trigger;
        break;
    case Weapon::InputMethod::LeftContinuous:
        inputOk = mouse->getState(Mouse::Button::Left) == ButtonState::Pressed;
        break;
    case Weapon::InputMethod::RightOneShot:
        inputOk = mouse->getState(Mouse::Button::Right) == ButtonState::Trigger;
        break;
    case Weapon::InputMethod::RightContinuous:
        inputOk = mouse->getState(Mouse::Button::Right) == ButtonState::Pressed;
        break;
    }
    if (inputOk) {
        m_mainWeaponFireRemain = m_mainWeapon->parameter.fireRate;
        m_mainWeapon->execute(chunk, std::static_pointer_cast<PlayerEntity>(shared_from_this()));
    }
}

void PlayerEntity::coolMainWeapon()
{
    if (!m_mainWeapon) {
        return;
    }
    if (m_mainWeaponCoolRemain > 0.0f) {
        m_mainWeaponCoolRemain = Mathf::max(0.0f, m_mainWeaponCoolRemain - Time::deltaTime());
        return;
    }
    m_mainWeaponCoolRemain = m_mainWeapon->parameter.coolRate;
    m_mainWeaponEnergy = Mathf::min(m_mainWeapon->getEnergyMax(), m_mainWeaponEnergy + m_mainWeapon->parameter.increaseEnergy);
}

void PlayerEntity::fireSubWeapon(const std::shared_ptr<Chunk>& chunk)
{
    if (!m_subWeapon) {
        return;
    }
    if (m_subWeaponFireRemain > 0.0f) {
        m_subWeaponFireRemain = Mathf::max(0.0f, m_subWeaponFireRemain - Time::deltaTime());
        return;
    }
    auto mouse = InputSystem::getInstance()->getMouse();
    auto inputMethod = m_subWeapon->getInputMethod();
    bool inputOk = false;
    switch (inputMethod) {
    case Weapon::InputMethod::None:
        break;
    case Weapon::InputMethod::LeftOneShot:
        inputOk = mouse->getState(Mouse::Button::Left) == ButtonState::Trigger;
        break;
    case Weapon::InputMethod::LeftContinuous:
        inputOk = mouse->getState(Mouse::Button::Left) == ButtonState::Pressed;
        break;
    case Weapon::InputMethod::RightOneShot:
        inputOk = mouse->getState(Mouse::Button::Right) == ButtonState::Trigger;
        break;
    case Weapon::InputMethod::RightContinuous:
        inputOk = mouse->getState(Mouse::Button::Right) == ButtonState::Pressed;
        break;
    }
    if (inputOk) {
        m_subWeaponFireRemain = m_subWeapon->parameter.fireRate;
        m_subWeapon->execute(chunk, std::static_pointer_cast<PlayerEntity>(shared_from_this()));
    }
}

void PlayerEntity::coolSubWeapon()
{
    if (!m_subWeapon) {
        return;
    }
    if (m_subWeaponCoolRemain > 0.0f) {
        m_subWeaponCoolRemain = Mathf::max(0.0f, m_subWeaponCoolRemain - Time::deltaTime());
        return;
    }
    m_subWeaponCoolRemain = m_subWeapon->parameter.coolRate;
    m_subWeaponEnergy = Mathf::min(m_subWeapon->getEnergyMax(), m_subWeaponEnergy + m_subWeapon->parameter.increaseEnergy);
}
}