#pragma once
#include <Graphics/FontMetrics.hpp>
#include <memory>

namespace Lib::Graphics {
class Texture;
class FontSprite {
public:
    explicit FontSprite(const std::shared_ptr<Texture>& texture,
        FontMetrics metrics);
    const std::shared_ptr<Texture> texture;
    const FontMetrics metrics;

private:
};
}
