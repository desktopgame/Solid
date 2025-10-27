#pragma once
#include <App/library.hpp>
#include <memory>
#include <picojson/picojson.h>

namespace App::Common::Graphics {
/**
 * ボックスのツリーによって表されるオブジェクトのある１ノードを表します。
 */
class Node : public std::enable_shared_from_this<Node> {
public:
    /**
     * 新しいインスタンスを生成して返します。
     * @return
     */
    static std::shared_ptr<Node> create();

    /**
     * 削除予定とマークされた子要素を削除します。
     */
    void update();

    /**
     * このノードと子階層のノードを描画します。
     * @param renderer
     * @param blendColor
     * @param blendRate
     */
    void draw(const std::shared_ptr<Renderer>& renderer, const Vector3& blendColor, float blendRate);

    /**
     * このノードを削除予定としてマークします。
     */
    void removeFromParent();

    /**
     * 位置や回転のキャッシュを再計算する必要があることをマークします。
     */
    void invalidate();

    /**
     * 位置や回転のキャッシュを再計算します。
     */
    void validate();

    /**
     * クローンを返します。（ディープコピー）
     * @return
     */
    std::shared_ptr<Node> clone() const;

    /**
     * 指定のファイルにJSONとして書き出します。
     * @param file
     * @param node
     */
    static void serialize(const std::string& file, const std::shared_ptr<Node>& node);

    /**
     * 指定のファイルからJSONとして読み出します。
     * @param file
     * @return
     */
    static std::shared_ptr<Node> deserialize(const std::string& file);

    /**
     * このノードの名前を設定します。デバッグ用です。
     * @param name
     */
    void setName(const std::string& name);

    /**
     * このノードの名前を設定します。デバッグ用です。
     * @param name
     */
    void setName(const std::array<char, 32>& name);

    /**
     * このノードの名前を返します。
     * @return
     */
    std::array<char, 32> getName() const;

    /**
     * このノードの名前を返します。
     * @return
     */
    std::array<char, 32>& getName();

    /**
     * このノードのローカル座標を設定します。
     * @param localPosition
     */
    void setLocalPosition(const Vector3& localPosition);

    /**
     * このノードのローカル座標を返します。
     * @return
     */
    Vector3 getLocalPosition() const;

    /**
     * このノードのグローバル座標を返します。
     * @return
     */
    Vector3 getGlobalPosition();

    /**
     * このノードのローカル回転を設定します。
     * @param localRotation
     */
    void setLocalRotation(const Vector3& localRotation);

    /**
     * このノードのローカル回転を返します。
     * @return
     */
    Vector3 getLocalRotation() const;

    /**
     * このノードのサイズを設定します。
     */
    void setSize(const Vector3& size);

    /**
     * このノードのサイズを返します。
     * @return
     */
    Vector3 getSize() const;

    /**
     * このノードの色を設定します。
     * @param color
     */
    void setColor(const Vector3& color);

    /**
     * このノードの色を返します。
     * @return
     */
    Vector3 getColor() const;

    /**
     * 位置、回転を表すローカル行列を返します。
     * @return
     */
    Matrix getLocalTransform() const;

    /**
     * 位置、回転を表すローカル行列を計算して返します。
     * @return
     */
    Matrix computeLocalTransform();

    /**
     * 位置、回転を表すグローバル行列を返します。
     * @return
     */
    Matrix getGlobalTransform() const;

    /**
     * 位置、回転を表すグローバル行列を計算して返します。
     * @return
     */
    Matrix computeGlobalTransform();

    /**
     * 回転を表すグローバル行列を返します。
     * @return
     */
    Matrix getGlobalRotation() const;

    /**
     * 回転を表すグローバル行列を計算して返します。
     * @return
     */
    Matrix computeGlobalRotation();

    /**
     * エッジ座標を返します。
     * @return
     */
    std::array<Vector3, 8> getEdges() const;

    /**
     * エッジ座標を計算して返します。
     * @return
     */
    std::array<Vector3, 8> computeEdges();

    /**
     * OBBを返します。
     * @return
     */
    Geom::OBB getOBB() const;

    /**
     * OBBを計算して返します。
     * @return
     */
    Geom::OBB computeOBB();

    /**
     * 子要素を追加します。
     * @param node
     */
    void addChild(const std::shared_ptr<Node>& node);

    /**
     * 指定位置の子要素を返します。
     * @param index
     * @return
     */
    std::shared_ptr<Node> getChildAt(int32_t index) const;

    /**
     * 子要素の数を返します。
     * @return
     */
    int32_t getChildrenCount() const;

    bool isRemoved() const;

private:
    static void clone(const std::shared_ptr<const Node>& src, const std::shared_ptr<Node>& dst);
    static void serialize(picojson::value::object& parent, const std::shared_ptr<Node>& node);
    static void deserialize(picojson::value::object& parent, const std::shared_ptr<Node>& node);

    Node();
    void draw(const std::shared_ptr<Node>& parent, const std::shared_ptr<Renderer>& renderer, const Vector3& blendColor, float blendRate);

    std::array<char, 32> m_name;
    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_size;
    Vector3 m_color;
    Matrix m_localTransform;
    Matrix m_globalTransform;
    Matrix m_globalRotation;
    std::array<Vector3, 8> m_edges;
    Geom::OBB m_obb;
    bool m_isInvalid;
    std::weak_ptr<Node> m_parent;
    std::vector<std::shared_ptr<Node>> m_children;
    bool m_removed;
};
}