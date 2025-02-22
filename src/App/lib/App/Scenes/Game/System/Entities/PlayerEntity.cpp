#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entities/ProjectileEntity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <App/Scenes/Game/System/WeaponRegistry.hpp>
#include <App/Scenes/Game/System/Weapons/SingleOneShotWeapon.hpp>
#include <imgui.h>

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
void PlayerEntity::onGui()
{
    ImGui::Begin("Player");
    if (m_mainWeapon) {
        ImGui::LabelText("M_Energy", "%f/%f", m_mainWeaponEnergy, m_mainWeapon->getEnergyMax());
        ImGui::LabelText("M_FireRemain", "%f", m_mainWeaponFireRemain);
        ImGui::LabelText("M_CoolRemain", "%f", m_mainWeaponCoolRemain);
    }
    if (m_subWeaponEnergy) {
        ImGui::LabelText("S_Energy", "%f/%f", m_subWeaponEnergy, m_subWeapon->getEnergyMax());
        ImGui::LabelText("S_FireRemain", "%f", m_subWeaponFireRemain);
        ImGui::LabelText("S_CoolRemain", "%f", m_subWeaponCoolRemain);
    }
    ImGui::End();
}
void PlayerEntity::draw3D(const std::shared_ptr<Renderer>& renderer) { }
void PlayerEntity::draw2D(const std::shared_ptr<Renderer>& renderer) { }

float PlayerEntity::getCameraAngleX() const { return m_cameraAngleX; }
float PlayerEntity::getCameraAngleY() const { return m_cameraAngleY; }
Vector3 PlayerEntity::getForward() const
{
    auto rotation = Quaternion::angleAxis(m_cameraAngleY, Vector3({ 0, 1, 0 })) * Quaternion::angleAxis(-m_cameraAngleX, Vector3({ 1, 0, 0 }));
    auto forward = Quaternion::transform(rotation, Vector3({ 0, 0, 1 }));
    return forward;
}

void PlayerEntity::setMainWeapon(const std::shared_ptr<Weapon>& mainWeapon)
{
    m_mainWeapon = mainWeapon;
    m_mainWeaponEnergy = mainWeapon->getEnergyMax();
    m_mainWeaponFireRemain = 0.0f;
    m_mainWeaponCoolRemain = 0.0f;
}
std::shared_ptr<Weapon> PlayerEntity::getMainWeapon() const { return m_mainWeapon; }

void PlayerEntity::setSubWeapon(const std::shared_ptr<Weapon>& subWeapon)
{
    m_subWeapon = subWeapon;
    m_subWeaponEnergy = subWeapon->getEnergyMax();
    m_subWeaponFireRemain = 0.0f;
    m_subWeaponCoolRemain = 0.0f;
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
    setMainWeapon(WeaponRegistry::s_singleOneShotWeaponLv1);
    setSubWeapon(WeaponRegistry::s_singleOneShotWeaponLv1);
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
    case Weapon::InputMethod::OneShot:
        inputOk = mouse->getState(Mouse::Button::Left) == ButtonState::Trigger;
        break;
    case Weapon::InputMethod::Continuous:
        inputOk = mouse->getState(Mouse::Button::Left) == ButtonState::Pressed;
        break;
    }
    if (inputOk) {
        if (m_mainWeaponEnergy >= m_mainWeapon->parameter.decreaseEnergy) {
            m_mainWeaponEnergy = Mathf::max(0.0f, m_mainWeaponEnergy - m_mainWeapon->parameter.decreaseEnergy);
            m_mainWeaponFireRemain = m_mainWeapon->parameter.fireRate;
            m_mainWeapon->execute(chunk, std::static_pointer_cast<PlayerEntity>(shared_from_this()));
        }
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
    case Weapon::InputMethod::OneShot:
        inputOk = mouse->getState(Mouse::Button::Right) == ButtonState::Trigger;
        break;
    case Weapon::InputMethod::Continuous:
        inputOk = mouse->getState(Mouse::Button::Right) == ButtonState::Pressed;
        break;
    }
    if (inputOk) {
        if (m_subWeaponEnergy >= m_subWeapon->parameter.decreaseEnergy) {
            m_subWeaponEnergy = Mathf::max(0.0f, m_subWeaponEnergy - m_subWeapon->parameter.decreaseEnergy);
            m_subWeaponFireRemain = m_subWeapon->parameter.fireRate;
            m_subWeapon->execute(chunk, std::static_pointer_cast<PlayerEntity>(shared_from_this()));
        }
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