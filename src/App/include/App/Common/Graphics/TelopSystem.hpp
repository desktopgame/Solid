#pragma once
#include <App/library.hpp>

namespace App::Common::Graphics {
class Telop;
/**
 * 全てのテロップを一元管理するクラスです。
 */
class TelopSystem {
public:
    /**
     * テロップシステムを初期化します。
     */
    static void initialize();

    /**
     * テロップを追加します。
     * @param text
     * @param position
     * @param color
     * @param duration
     */
    static void request(const std::u16string& text, const Vector3& position, const Vector3& color, float duration);

    /**
     * 全てのテロップを描画します。
     * 寿命を迎えたテロップは自動でプールに返されます。
     */
    static void draw();

    /**
     * 全てのテロップを停止してプールへ返します。
     */
    static void stop();

    /**
     * テロップシステムを破棄します。
     */
    static void destroy();

private:
    TelopSystem() = delete;

    static std::vector<std::shared_ptr<Telop>> s_freeTable;
    static std::vector<std::shared_ptr<Telop>> s_usedTable;

    static std::shared_ptr<FontMap> s_fontMap;
    static std::shared_ptr<CpuBuffer> s_vertexBuffer;
    static std::shared_ptr<CpuBuffer> s_indexBuffer;
    static int32_t s_indexLength;
};
}