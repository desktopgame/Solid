#pragma once
#include <Graphics/FontMap.hpp>
#include <Graphics/Renderer.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::UI {
class Container;
/**
 * コンポーネントは、ユーザーと対話するためのグラフィカルなインターフェイスです。
 * 例えば、ボタン、ラベル、スクロールなどがあります。
 */
class Component : public std::enable_shared_from_this<Component> {
public:
    explicit Component();
    virtual ~Component();

    /**
     * このコンポーネントに対する入力を処理します。
     */
    virtual void update() = 0;

    /**
     * このコンポーネントを描画します。
     * @param renderer
     */
    virtual void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) = 0;

    /**
     * 親コンテナーを設定します。
     * @param parent
     */
    void setParent(const std::shared_ptr<Container>& parent);

    /**
     * 親コンテナーを返します。
     * @return
     */
    std::shared_ptr<Container> getParent() const;

    /**
     * コンポーネントのサイズを設定します。
     * トップレベルのコンテナーでは必ずこれを呼び出す必要があります。
     * @param size
     */
    void setSize(const Math::Vector2& size);

    /**
     * コンポーネントのサイズを返します。
     * @return
     */
    Math::Vector2 getSize() const;

    /**
     * コンポーネントの親コンテナーの中心からのローカル座標を設定します。
     * @param position
     */
    void setPosition(const Math::Vector2& position);

    /**
     * コンポーネントの親コンテナーの中心からのローカル座標を返します。
     * @return
     */
    Math::Vector2 getPosition() const;

    /**
     * ウィンドウ上のグローバルなコンポーネントの中心座標を返します。
     * @return
     */
    Math::Vector2 getGlobalPosition() const;

    /**
     * 任意のグローバルな点がこのコンポーネントに含まれるなら true を返します。
     * @param position
     * @return
     */
    bool isContains(const Math::Vector2& position);

    /**
     * コンポーネントの最小サイズを設定します。
     * @param minimumSize
     */
    void setMinimumSize(const Math::Vector2& minimumSize);

    /**
     * コンポーネントの最小サイズを返します。
     * @return
     */
    Math::Vector2 getMinimumSize() const;

    /**
     * コンポーネントの最大サイズを設定します。
     * @param maximumSize
     */
    void setMaximumSize(const Math::Vector2& maximumSize);

    /**
     * コンポーネントの最大サイズを返します。
     * @return
     */
    Math::Vector2 getMaximumSize() const;

    /**
     * コンポーネントの推奨サイズを設定します。
     * コンポーネントにおいては必ずこれを呼び出してください。
     * コンテナーの場合は、レイアウトマネージャが決定します。
     * @param preferredSize
     * @return
     */
    void setPreferredSize(const Math::Vector2& preferredSize);

    /**
     * コンポーネントの推奨サイズを返します。
     * @return
     */
    Math::Vector2 getPreferredSize() const;

    /**
     * このコンポーネントが親コンテナー内で伸縮可能なら true を設定します。
     * @param isFlexible
     */
    void setFlexible(bool isFlexible);

    /**
     * このコンポーネントが親コンテナー内で伸縮可能なら true を返します。
     * @return
     */
    bool isFlexible() const;

    /**
     * コンポーネントの前景色を設定します。
     * @param foregroundColor
     */
    void setForegroundColor(const Graphics::Color& foregroundColor);

    /**
     * コンポーネントの前景色を返します。
     * @return
     */
    Graphics::Color getForegroundColor() const;

    /**
     * コンポーネントの背景色を設定します。
     * @param backgroundColor
     */
    void setBackgroundColor(const Graphics::Color& backgroundColor);

    /**
     * コンポーネントの背景色を返します。
     * @return
     */
    Graphics::Color getBackgroundColor() const;

    /**
     * コンポーネントのフォントを設定します。
     * @param font
     */
    void setFont(const std::shared_ptr<Graphics::FontMap>& fontMap);

    /**
     * コンポーネントのフォントを返します。
     * @return
     */
    std::shared_ptr<Graphics::FontMap> getFont() const;

    /**
     * コンポーネントのフォントサイズを設定します。
     * @param fontSize
     */
    void setFontSize(int32_t fontSize);

    /**
     * コンポーネントのフォントサイズを返します。
     * @return
     */
    int32_t getFontSize() const;

private:
    std::weak_ptr<Container> m_parent;

    Math::Vector2 m_size;
    Math::Vector2 m_position;

    Math::Vector2 m_minimumSize;
    Math::Vector2 m_maximumSize;
    Math::Vector2 m_preferredSize;
    bool m_isFlexible;
    Graphics::Color m_foregroundColor;
    Graphics::Color m_backgroundColor;
    std::shared_ptr<Graphics::FontMap> m_fontMap;
    int32_t m_fontSize;
};
}