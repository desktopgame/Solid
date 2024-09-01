
#include <Graphics/Font.hpp>
#include <Graphics/FontFactory.hpp>
#include <Graphics/FontMap.hpp>
#include <cstdlib>
#include <freetype/freetype.h>

namespace Lib::Graphics {
// Impl
class FontFactory::Impl {
public:
    explicit Impl()
        : ft()
    {
    }
    FT_Library ft;
};
// FontFactory
FontFactory::Instance FontFactory::m_instance = nullptr;
FontFactory::~FontFactory()
{
    m_fontRegistry.clear();
    FT_Done_FreeType(m_impl->ft);
}
FontFactory::Instance FontFactory::getInstance()
{
    if (!FontFactory::m_instance) {
        FontFactory::m_instance = Instance(new FontFactory());
    }
    return FontFactory::m_instance;
}
std::shared_ptr<FontMap> FontFactory::load(const std::string& path)
{
    if (m_fontRegistry.count(path)) {
        return m_fontRegistry.at(path);
    }
    auto font = std::make_shared<Font>(reinterpret_cast<void*>(&(m_impl->ft)), path);
    auto fontMap = std::make_shared<FontMap>(font);
    m_fontRegistry[path] = fontMap;
    return fontMap;
}
bool FontFactory::isOccurredError() const { return m_occurredError; }
// private
FontFactory::FontFactory()
    : m_impl(std::unique_ptr<FontFactory::Impl>(new FontFactory::Impl()))
    , m_fontRegistry()
    , m_occurredError(false)
{
    if (FT_Init_FreeType(&m_impl->ft)) {
        // Debug::logWarning("FreeType is not working.");
        this->m_occurredError = true;
    }
}
}