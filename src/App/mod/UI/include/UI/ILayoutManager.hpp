#pragma once
#include <Math/Vector.hpp>
#include <UI/LayoutElement.hpp>
#include <memory>

namespace Lib::UI {
class Container;
class Component;
/**
 * コンテナー内におけるコンポーネントの整列方法を定義するレイアウト・マネージャです。
 * コンポーネント同士が重なり合うようにレイアウトされた場合の動作は未定義です。
 */
class ILayoutManager {
public:
    explicit ILayoutManager() = default;
    virtual ~ILayoutManager() = default;

    /**
     * コンテナーを親コンテナーから与えられた使用可能領域のサイズに基づいてリサイズします。
     * このメソッドはトップダウンで実行されます。
     * @param parent
     * @param availableSize
     */
    virtual void resizeContainer(const std::shared_ptr<Container>& parent, const Math::Vector2& availableSize) = 0;

    /**
     * コンテナー内のコンポーネントを整列させます。
     * このメソッドはボトムアップで実行されます。
     * @param parent
     */
    virtual void layoutContainer(const std::shared_ptr<Container>& parent) = 0;

    /**
     * コンテナーの推奨サイズを計算します。
     * あるコンテナーが子コンテナーのために割り当てるサイズを計算するとき、子コンテナー自身のサイズはまだ決定されていません。
     * そのため、このメソッドが返す推奨サイズを参考に割り当てサイズを決めることがあります。
     * @param parent
     * @return
     */
    virtual Math::Vector2 computePreferredSize(const std::shared_ptr<Container>& parent) = 0;

    /**
     * コンテナー内に配置されている子コンテナーのために使用可能サイズを返します。
     * 子コンテナーの resizeContainer() を呼び出すために、自身の resizeContainer() のあとで呼ばれます。
     * @param parent
     * @param container
     * @param hint
     */
    virtual Math::Vector2 availableSizeFor(const std::shared_ptr<Container>& parent, const std::shared_ptr<Container>& container, const std::shared_ptr<LayoutHint>& hint) = 0;

private:
};
}