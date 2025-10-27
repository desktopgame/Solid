#pragma once
#include <Graphics/FontMetrics.hpp>
#include <memory>

namespace Lib::Graphics {
class Texture;
/**
 * あるフォントの一文字のテクスチャを保持するクラスです。
 */
class FontSprite {
public:
    explicit FontSprite(const std::shared_ptr<Texture>& texture,
        FontMetrics metrics);
    const std::shared_ptr<Texture> texture;
    const FontMetrics metrics;

private:
};
}
