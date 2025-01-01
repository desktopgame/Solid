#include <Common/Graphics/Telop.hpp>

namespace App::Common::Graphics {
// public
Telop::Telop()
    : text()
    , position()
    , color()
    , duration()
    , m_elapsed()
{
}

void Telop::initialize()
{
    m_elapsed = 0.0f;
}

void Telop::update()
{
    if (!isExpired()) {
        m_elapsed += Time::deltaTime();
    }
}

void Telop::draw(const std::shared_ptr<FontMap>& fontMap, const std::shared_ptr<CpuBuffer>& vertexBuffer, const std::shared_ptr<CpuBuffer>& indexBuffer, int32_t indexLength)
{
    auto rc = RenderContext::get(Metadata::ProgramTable::Text2D);
    auto surface = Engine::getInstance()->getDevice()->getSurface();
    Vector2 offset;
    for (char16_t c : text) {
        auto fontSprite = fontMap->load(20, c);

        auto ub = UniformPool::rent(Metadata::Text2D);
        auto modelMatrix = Matrix::transform(
            Matrix::translate(position),
            Matrix(),
            Matrix::scale(Vector3({ static_cast<float>(fontSprite->metrics.size.x()), static_cast<float>(fontSprite->metrics.size.y()), 1.0f })));
        Reflect::UCamera uCamera;
        uCamera.modelMatrix = modelMatrix;
        uCamera.viewMatrix = Camera::getLookAtMatrix();
        uCamera.projectionMatrix = Camera::getPerspectiveMatrix();
        ub->setVS(0, &uCamera);

        Reflect::UVector4 uColor;
        uColor.value = Vector4(color, 1.0f);
        ub->setVS(1, &uColor);
        ub->setPS(0, fontSprite->texture);
        surface->render(rc, ub, vertexBuffer, indexBuffer, indexLength);
    }
}

void Telop::destroy()
{
}

bool Telop::isExpired() const { return m_elapsed >= duration; }
// private
}