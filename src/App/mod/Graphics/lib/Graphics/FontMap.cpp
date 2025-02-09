#include <Graphics/Font.hpp>
#include <Graphics/FontMap.hpp>
#include <Graphics/FontSprite.hpp>
#include <Graphics/Texture.hpp>
#include <algorithm>

namespace Lib::Graphics {
// Container
class FontMap::Container {
public:
    explicit Container(int32_t size)
        : size(size)
        , textureMap()
    {
    }
    int32_t size;
    std::unordered_map<char16_t, std::shared_ptr<FontSprite>> textureMap;
};
// FontMap
FontMap::FontMap(const std::shared_ptr<Font>& font)
    : m_font(font)
    , m_containerVec()
{
}
std::shared_ptr<FontSprite> FontMap::load(int32_t size, unsigned long charcode)
{
    auto iter = std::find_if(m_containerVec.begin(), m_containerVec.end(),
        [size](std::shared_ptr<Container> c) -> bool {
            return size == c->size;
        });
    if (iter == m_containerVec.end()) {
        auto c = std::make_shared<Container>(size);
        m_containerVec.emplace_back(c);
        return load(size, charcode);
    }
    auto c = *iter;
    if (c->textureMap.count(charcode)) {
        return c->textureMap.at(charcode);
    }
    m_font->load(size, charcode);
    auto fontInstance = m_font->getCurrentInstance();
    auto texture = Texture::create(
        fontInstance->metrics.size.x(),
        fontInstance->metrics.size.y(), Texture::Format::Red, fontInstance->data);
    auto fontSprite = std::make_shared<FontSprite>(texture, fontInstance->metrics);
    c->textureMap[charcode] = fontSprite;
    return fontSprite;
}
std::vector<std::shared_ptr<FontSprite>> FontMap::load(
    int32_t size, const std::u16string& str)
{
    std::vector<std::shared_ptr<FontSprite>> v;
    for (char16_t c : str) {
        v.emplace_back(load(size, c));
    }
    return v;
}
Math::Vector2 FontMap::measure(int32_t size, const std::u16string& text, bool ignoreLineSpace)
{
    auto fontSprites = load(size, text);
    Math::Vector2 offset({ 0, 0 });
    float maxY = -1;
    for (auto fontSprite : fontSprites) {
        auto size = fontSprite->metrics.size.y();
        if (ignoreLineSpace) {
            size = (fontSprite->metrics.size.y() - fontSprite->metrics.bearing.y());
        }
        if (maxY < size) {
            maxY = size;
        }
    }
    offset.y() = maxY;
    for (auto fontSprite : fontSprites) {
        offset.x() += fontSprite->metrics.advance.x() >> 6;
    }
    return offset;
}
}