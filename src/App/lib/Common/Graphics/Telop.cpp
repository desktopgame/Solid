#include <Common/Graphics/Telop.hpp>
#include <iostream>

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
    // ワールド空間をスクリーン空間に変換
    Vector4 ndcPos = Vector4(position, 1.0f);
    ndcPos = Matrix::multiply(Camera::getLookAtMatrix(), ndcPos);
    ndcPos = Matrix::multiply(Camera::getPerspectiveMatrix(), ndcPos);

    if (ndcPos.w() != 0.0f) {
        ndcPos.x() /= ndcPos.w();
        ndcPos.y() /= ndcPos.w();
    }

    float screenX = (ndcPos.x() * 0.5f) * Screen::getWidth();
    float screenY = -(ndcPos.y() * 0.5f) * Screen::getHeight();
    // 長さを測って左端にオフセット
    float stringWidth = 0.0f;
    for (char16_t c : text) {
        stringWidth += fontMap->load(32, c)->metrics.size.x();
    }
    screenX -= (stringWidth / 2.0f);
    // フォントを描画
    auto rc = RenderContext::get(Metadata::ProgramTable::Text2D);
    auto surface = Engine::getInstance()->getDevice()->getSurface();
    float offsetY = (m_elapsed / duration) * 100.0f;
    for (char16_t c : text) {
        auto fontSprite = fontMap->load(32, c);
        auto ub = UniformPool::rent(Metadata::Text2D);

        auto modelMatrix = Matrix::transform(
            Matrix::translate(Vector3({ screenX, screenY + offsetY, 0.0f })),
            Matrix(),
            Matrix::scale(Vector3({ static_cast<float>(fontSprite->metrics.size.x()), static_cast<float>(fontSprite->metrics.size.y()), 1.0f })));
        Reflect::UCamera uCamera;
        uCamera.modelMatrix = modelMatrix;
        uCamera.viewMatrix = Matrix();
        uCamera.projectionMatrix = Camera::getOrthoMatrix();
        ub->setVS(0, &uCamera);

        Reflect::UVector4 uColor;
        uColor.value = Vector4(color, 1.0f - (m_elapsed / duration));
        ub->setVS(1, &uColor);
        ub->setPS(0, fontSprite->texture);
        surface->render(rc, ub, vertexBuffer, indexBuffer, indexLength);
        screenX += fontSprite->metrics.size.x();
    }
}

void Telop::destroy()
{
}

bool Telop::isExpired() const { return m_elapsed >= duration; }
// private
}