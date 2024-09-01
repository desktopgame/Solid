#pragma once
#include <Graphics/FontMetrics.hpp>
#include <memory>

namespace Lib::Graphics {
class Texture;
class FontSprite {
public:
    using Reference = std::weak_ptr<FontSprite>;
    using Instance = std::shared_ptr<FontSprite>;
    explicit FontSprite(const std::shared_ptr<Texture>& texture,
        FontMetrics metrics);
    const std::shared_ptr<Texture> texture;
    const FontMetrics metrics;

private:
};
}
