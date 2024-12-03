#include <Graphics/MatrixStack.hpp>

namespace Lib::Graphics {

MatrixStack::MatrixStack()
    : m_stack()
{
}
void MatrixStack::push(const Math::Matrix& m)
{
    m_stack.emplace_back(m);
}
Math::Matrix MatrixStack::top() const { return m_stack.back(); }
void MatrixStack::pop() { m_stack.pop_back(); }
Math::Matrix MatrixStack::mult() const
{
    Math::Matrix m;
    auto iter = m_stack.begin();
    while (iter != m_stack.end()) {
        m = m * (*iter);
        ++iter;
    }
    return m;
}
void MatrixStack::clear()
{
    m_stack.clear();
}
}