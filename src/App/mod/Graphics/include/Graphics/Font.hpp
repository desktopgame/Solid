#pragma once
#include <Graphics/FontMetrics.hpp>
#include <memory>
#include <string>

namespace Lib::Graphics {
class FontInstance {
public:
    explicit FontInstance(const unsigned char* data,
        const FontMetrics& metrics);
    const unsigned char* data;
    const FontMetrics metrics;

private:
};
class Font {
public:
    using Reference = std::weak_ptr<Font>;
    using Instance = std::shared_ptr<Font>;
    explicit Font(void* ft, const std::string& path);
    ~Font();
    void load(int size, unsigned long charcode);
    std::shared_ptr<FontInstance> getCurrentInstance();
    bool isOccurredError() const;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
    bool m_occurredError;
    bool m_textureCreated;
    int m_size;
};
}