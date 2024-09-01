#include <Graphics/FontSprite.hpp>
namespace Lib::Graphics {
FontSprite::FontSprite(const std::shared_ptr<Texture>& texture,
    FontMetrics metrics)
    : texture(texture)
    , metrics(metrics)
{
}
}