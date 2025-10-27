#pragma once
#include <Graphics/FontMetrics.hpp>
#include <memory>
#include <string>

namespace Lib::Graphics {
/**
 * あるフォントの一文字のテクスチャを保持するクラスです。
 */
class FontInstance {
public:
    explicit FontInstance(const unsigned char* data,
        const FontMetrics& metrics);
    const unsigned char* data;
    const FontMetrics metrics;

private:
};
/**
 * フォントのロード処理を行うクラスです。
 */
class Font {
public:
    explicit Font(void* ft, const std::string& path);
    ~Font();

    /**
     * 指定のフォントサイズで指定の文字をロードします。
     * テクスチャを取得するには getCurrentInstance() を呼び出してください。
     * @param size
     * @param charcode
     */
    void load(int32_t size, unsigned long charcode);

    /**
     * 最後にロードしたフォント情報を返します。
     * @return
     */
    std::shared_ptr<FontInstance> getCurrentInstance();

    /**
     * フォントに関するエラーが発生していれば true を返します。
     * @return
     */
    bool isOccurredError() const;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
    bool m_occurredError;
    bool m_textureCreated;
    int32_t m_size;
};
}