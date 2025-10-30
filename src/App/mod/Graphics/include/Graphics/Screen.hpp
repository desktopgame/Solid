#pragma once
#include <Math/Vector.hpp>
#include <numeric>

namespace Lib::Graphics {
/**
 * スクリーンの解像度を扱うクラスです。
 */
class Screen {
public:
    /**
     * ウィンドウサイズを設定します。
     * エンジンを起動する前に設定する必要があります。
     * ウィンドウのリサイズは不可能です。
     * @param size
     */
    static void setSize(const Math::IntVector2& size);

    /**
     * ウィンドウサイズを返します。
     * @return
     */
    static Math::IntVector2 getSize();

    /**
     * ウィンドウサイズを設定します。
     * エンジンを起動する前に設定する必要があります。
     * ウィンドウのリサイズは不可能です。
     * @param width
     * @param height
     */
    static void setSize(int32_t width, int32_t height);

    /**
     * ウィンドウの横幅を返します。
     * @return
     */
    static int32_t getWidth();

    /**
     * ウィンドウの縦幅を返します。
     * @return
     */
    static int32_t getHeight();

    /**
     * ウィンドウのアスペクト比を返します。
     * @return
     */
    static float getAspectRatio();

private:
    static int32_t s_width;
    static int32_t s_height;

    Screen() = delete;
    ~Screen() = delete;
};
}