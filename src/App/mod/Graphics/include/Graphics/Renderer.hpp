#pragma once
#include <Graphics/Color.hpp>
#include <Graphics/FontMap.hpp>
#include <Graphics/MatrixStack.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <memory>

namespace Lib::Graphics {
class CpuBuffer;
class Texture;
class TileBatch;
class RenderContext;
class UniformBuffer;
/**
 * レンダーコンテキストをラップし、簡単に描画を実行できるようにしたクラスです。
 * このクラスはものすごく狭いケースのユースケースしかサポートしません。
 * が、シンプルなアプリケーションやUI実装では役に立ちます。
 */
class Renderer {
public:
    enum class TextAlignX {
        Center = 0,
        Left,
        Right
    };
    enum class TextAlignY {
        Center = 0,
        Top,
        Bottom
    };

    explicit Renderer();

    /**
     * 行列をプッシュし、以後の描画のモデル行列に適用するようにします。
     * @param m
     */
    void pushMatrix(const Math::Matrix& m);

    /**
     * 行列をポップします。
     */
    void popMatrix();

    /**
     * 文字の描画に使用するフォントを変更します。
     * @param fontMap
     */
    void textFont(const std::shared_ptr<FontMap>& fontMap);

    /**
     * 文字の描画に使用するフォントサイズを変更します。
     * @param fontSize
     */
    void textFontSize(int32_t fontSize);

    /**
     * ステンシル読み取りモードにします。
     * 2Dの描画でのみサポートされます。
     * ステンシルが一致するときだけ描画します。
     */
    void stencilRead();

    /**
     * ステンシル書き込みモードにします。
     * 2Dの描画でのみサポートされます。
     */
    void stencilWrite();

    /**
     * ステンシルの読み込み・書き込みをオフにします。
     */
    void stencilNone();

    /**
     * ステンシル参照値を設定します。
     * @param value
     */
    void stencilRef(uint32_t value);

    /**
     * ステンシルをクリアします。
     */
    void stencilClear();

    /**
     * 矩形を描画します。
     * @param position
     * @param size
     * @param degree
     * @param color
     */
    void drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color);

    /**
     * 円を描画します。
     * @param position
     * @param size
     * @param color
     */
    void drawCircle(const Math::Vector2& position, const Math::Vector2& size, const Color& color);

    /**
     * スプライトを描画します。
     * @param position
     * @param size
     * @param degree
     * @param texture
     * @param color
     */
    void drawSprite(const Math::Vector2& position, const Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color);

    /**
     * テキストを描画します。
     * @param position
     * @param alignX
     * @param alignY
     * @param label
     * @param color
     */
    void drawText(const Math::Vector2& position, TextAlignX alignX, TextAlignY alignY, const std::u16string& label, const Color& color);

    /**
     * テキストのサイズを返します。
     * @param label
     * @param alignY
     * @return
     */
    Math::Vector2 measureText(const std::u16string& label, TextAlignY alignY);

    /**
     * プレーンを描画します。
     * @param position
     * @param scale
     * @param rotation
     * @param color
     * @param isWireframe
     */
    void drawPlane(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const Math::Vector4& color, bool isWireframe);

    /**
     * プレーンの線のみを描画します。
     * @param position
     * @param scale
     * @param rotation
     * @param color
     * @param lineWidth
     */
    void drawPlaneLine(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const Math::Vector4& color, float lineWidth);

    /**
     * テクスチャ付きのプレーンを描画します。
     * @param position
     * @param scale
     * @param rotation
     * @param texture
     * @param color
     */
    void drawPlaneTexture(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color);

    /**
     * ボックスを描画します。
     * @param position
     * @param scale
     * @param rotation
     * @param color
     * @param isWireframe
     */
    void drawBox(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const Math::Vector4& color, bool isWireframe);

    /**
     * ボックスの線のみを描画します。
     * @param position
     * @param scale
     * @param rotation
     * @param color
     * @param lineWidth
     */
    void drawBoxLine(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const Math::Vector4& color, float lineWidth);

    /**
     * テクスチャ付きのボックスを描画します。
     * @param position
     * @param scale
     * @param rotation
     * @param texture
     * @param color
     */
    void drawBoxTexture(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color);

private:
    class Object {
    public:
        explicit Object() = default;
        std::shared_ptr<RenderContext> rc;
        std::shared_ptr<CpuBuffer> vertexBuffer;
        std::shared_ptr<CpuBuffer> indexBuffer;
        int32_t indexLength;
    };

    void initRect();
    void initCircle();
    void initSprite();
    void initText();

    void initPlane(Object& dst, bool isWireframe);
    void initPlaneLine();
    void initPlaneTexture();
    void initBox(Object& dst, bool isWireframe);
    void initBoxLine();
    void initBoxTexture();

    Math::Matrix applyMatrix(const Math::Matrix& m);
    void renderObject(const Object& object, const std::shared_ptr<UniformBuffer>& ub);

    // common
    MatrixStack m_matrixStack;

    // text
    std::shared_ptr<FontMap> m_fontMap;
    int32_t m_fontSize;

    // stencil
    int32_t m_stencilMode;

    Object m_rectObject;
    Object m_rectSwObject;
    Object m_rectSrObject;
    Object m_circleObject;
    Object m_circleSwObject;
    Object m_circleSrObject;
    Object m_spriteObject;
    Object m_spriteSwObject;
    Object m_spriteSrObject;
    Object m_textObject;
    Object m_textSwObject;
    Object m_textSrObject;
    Object m_planeObject;
    Object m_planeWireframeObject;
    Object m_planeLineObject;
    Object m_planeTextureObject;
    Object m_boxObject;
    Object m_boxWireframeObject;
    Object m_boxLineObject;
    Object m_boxTextureObject;
};
}
