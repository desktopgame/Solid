#include <UI/ImageIcon.hpp>

namespace Lib::UI {

ImageIcon::ImageIcon()
    : Component()
{
}

void ImageIcon::update() { }
void ImageIcon::draw2D(const std::shared_ptr<Graphics::Renderer>& renderer)
{
    if (m_texture) {
        renderer->drawSprite(getGlobalPosition(), getSize(), 0.0f, m_texture, Graphics::Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
    }
}

void ImageIcon::setTexture(const std::shared_ptr<Graphics::Texture>& texture) { m_texture = texture; }
std::shared_ptr<Graphics::Texture> ImageIcon::getTexture() const { return m_texture; }
}