#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Lib::Graphics {
class Font;
class FontSprite;
class Texture;
class FontMap {
public:
    explicit FontMap(const std::shared_ptr<Font>& font);
    std::vector<std::shared_ptr<FontSprite>> load(
        int size, const std::u16string& str);
    std::shared_ptr<FontSprite> load(int size, unsigned long charcode);

private:
    class Container;
    std::shared_ptr<Font> m_font;
    std::vector<std::shared_ptr<Container>> m_containerVec;
};
}
