#pragma once
#include <UI/Component.hpp>
#include <UI/ILayoutManager.hpp>
#include <UI/LayoutElement.hpp>
#include <memory>
#include <vector>

namespace Lib::UI {
/**
 * 他のコンポーネントを含むことが出来るコンポーネントです。
 */
class Container : public Component {
public:
    explicit Container();
    virtual ~Container();

    virtual void update() override;
    virtual void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    /**
     * レイアウト・マネージャを呼び出し、コンポーネントを整列させます。
     * 自動では実行されない点に注意してください。
     */
    void doLayout();

    /**
     * コンテナーのレイアウト・マネージャを設定します。
     * @param layoutManager
     */
    virtual void setLayout(const std::shared_ptr<ILayoutManager>& layoutManager);

    /**
     * コンテナーのレイアウト・マネージャを返します。
     * @return
     */
    virtual std::shared_ptr<ILayoutManager> getLayout();

    /**
     * コンテナー内に配置される要素を追加します。
     * @param layoutElement
     */
    virtual void addLayoutElement(const std::shared_ptr<LayoutElement>& layoutElement);

    /**
     * コンテナーから要素を削除します。
     * @param index
     */
    virtual void removeLayoutElementAt(int32_t index);

    /**
     * コンテナー内の要素を返します。
     * @param index
     * @return
     */
    virtual std::shared_ptr<LayoutElement> getLayoutElementAt(int32_t index) const;

    /**
     * コンテナー内の要素数を返します。
     * @return
     */
    virtual int32_t getLayoutElementCount() const;

private:
    void layoutMeasure(const Math::Vector2& availableSize);
    void layoutArrange();

    std::shared_ptr<ILayoutManager> m_layoutManager;
    std::vector<std::shared_ptr<LayoutElement>> m_children;
};
}