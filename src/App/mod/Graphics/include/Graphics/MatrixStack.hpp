#pragma once
#include <Math/Matrix.hpp>
#include <vector>

namespace Lib::Graphics {
class MatrixStack {
public:
    explicit MatrixStack();
    void push(const Math::Matrix& m);
    Math::Matrix top() const;
    void pop();
    Math::Matrix mult();
    void clear();

private:
    std::vector<Math::Matrix> m_stack;
    Math::Matrix m_mult;
    bool m_multDirty;
};
}