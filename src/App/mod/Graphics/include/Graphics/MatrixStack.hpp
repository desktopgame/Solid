#pragma once
#include <Math/Matrix.hpp>
#include <vector>

namespace Lib::Graphics {
/**
 * 行列を階層的に管理するスタックです。
 */
class MatrixStack {
public:
    explicit MatrixStack();

    /**
     * 新たな階層をプッシュします。
     * @param m
     */
    void push(const Math::Matrix& m);

    /**
     * トップの行列を返します。
     * @return
     */
    Math::Matrix top() const;

    /**
     * 階層を一つ取り出します。
     */
    void pop();

    /**
     * 全ての階層を乗算したグローバルな行列を返します。
     */
    Math::Matrix mult();

    /**
     * スタックを空にします。
     */
    void clear();

private:
    std::vector<Math::Matrix> m_stack;
    Math::Matrix m_mult;
    bool m_multDirty;
};
}