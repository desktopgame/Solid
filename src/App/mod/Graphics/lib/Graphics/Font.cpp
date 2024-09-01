#include <Graphics/Font.hpp>
#include <cstdlib>
#include <freetype/freetype.h>

namespace Lib::Graphics {
// FontInstance
FontInstance::FontInstance(const unsigned char* data,
    const FontMetrics& metrics)
    : data(data)
    , metrics(metrics)
{
}
// Impl
class Font::Impl {
public:
    explicit Impl()
        : face()
    {
    }
    FT_Face face;
};
// Font
Font::Font(void* ft, const std::string& path)
    : m_impl(std::unique_ptr<Font::Impl>(new Font::Impl()))
    , m_occurredError(false)
    , m_textureCreated(false)
    , m_size(-1)
{
    this->m_occurredError = FT_New_Face(*(static_cast<FT_Library*>(ft)),
        path.c_str(), 0, &m_impl->face);
}
Font::~Font() { FT_Done_Face(m_impl->face); }
void Font::load(int32_t size, unsigned long charcode)
{
    if (this->m_size != size) {
        this->m_size = size;
        FT_Set_Pixel_Sizes(m_impl->face, 0, size);
    }
    if (FT_Load_Char(m_impl->face, charcode, FT_LOAD_RENDER)) {
        // Debug::logWarning("failed load a chareacter.");
        return;
    }
    this->m_textureCreated = true;
}
std::shared_ptr<FontInstance> Font::getCurrentInstance()
{
    if (this->m_occurredError || !m_textureCreated) {
        throw std::logic_error("texture is not created");
    }
    FT_Face& face = m_impl->face;
    FT_GlyphSlot& glyph = face->glyph;

    FontMetrics metrics(Math::makeVector2<int32_t>(glyph->bitmap.width, glyph->bitmap.rows),
        Math::makeVector2<int32_t>(glyph->bitmap_left, glyph->bitmap_top),
        Math::makeVector2<int32_t>(glyph->advance.x, glyph->advance.y));
    auto instance = std::make_shared<FontInstance>(glyph->bitmap.buffer, metrics);
    this->m_textureCreated = false;
    return instance;
}
bool Font::isOccurredError() const { return m_occurredError; }
}