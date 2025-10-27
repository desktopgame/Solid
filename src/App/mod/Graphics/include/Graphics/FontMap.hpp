#pragma once
#include <Math/Vector.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Lib::Graphics {
class Font;
class FontSprite;
class Texture;
/**
 * あるフォントの全ての文字とテクスチャの対応表を持つクラスです。
 */
class FontMap {
public:
    explicit FontMap(const std::shared_ptr<Font>& font);

    /**
     * 指定の文字列からフォントスプライトをロードして返します。
     * @param size
     * @param str
     * @return
     */
    std::vector<std::shared_ptr<FontSprite>> load(
        int32_t size, const std::u16string& str);

    /**
     * 指定の文字からフォントスプライトをロードして返します。
     * @param size
     * @param charcode
     * @return
     */
    std::shared_ptr<FontSprite> load(int32_t size, unsigned long charcode);

    /**
     * 指定の文字列を描画するのに必要なサイズを計算して返します。
     * @param size
     * @param text
     * @param ignoreLineSpace
     * @return
     */
    Math::Vector2 measure(int32_t size, const std::u16string& text, bool ignoreLineSpace);

    /**
     * 指定の文字列を描画するのに必要なサイズを計算して返します。
     * @param size
     * @param text
     * @param ignoreLineSpace
     * @return
     */
    Math::Vector2 measure(int32_t size, const std::u16string& text);

private:
    class Container;
    std::shared_ptr<Font> m_font;
    std::vector<std::shared_ptr<Container>> m_containerVec;
};
}
