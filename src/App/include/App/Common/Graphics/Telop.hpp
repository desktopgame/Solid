#pragma once
#include <App/library.hpp>
#include <memory>

namespace App::Common::Graphics {
/**
 * テロップを表示するクラスです。
 * ダメージ表示などに用いられます。
 */
class Telop {
public:
    static inline constexpr int32_t k_fontSize = 32;

    explicit Telop();

    /**
     * テロップを初期化します。
     */
    void initialize();

    /**
     * テロップを更新します。
     * 主に寿命管理がここで行われます。
     */
    void update();

    /**
     * 引数の矩形の頂点情報を使用してテロップを描画します。
     * @param fontMap
     * @param vertexBuffer
     * @param indexBuffer
     * @param indexLength
     */
    void draw(const std::shared_ptr<FontMap>& fontMap, const std::shared_ptr<CpuBuffer>& vertexBuffer, const std::shared_ptr<CpuBuffer>& indexBuffer, int32_t indexLength);

    /**
     * テロップを破棄します。
     */
    void destroy();

    /**
     * 寿命が切れたなら true を返します。
     * @return
     */
    bool isExpired() const;

    std::u16string text;
    Vector3 position;
    Vector3 color;
    float duration;

private:
    float m_elapsed;
};
}