#include <Graphics/MatrixStack.hpp>

namespace Lib::Graphics {

MatrixStack::MatrixStack()
    : m_stack()
    , m_mult()
    , m_multDirty(true)
{
}
void MatrixStack::push(const Math::Matrix& m)
{
    m_multDirty = true;
    m_stack.emplace_back(m);
}
Math::Matrix MatrixStack::top() const { return m_stack.back(); }
void MatrixStack::pop()
{
    m_multDirty = true;
    m_stack.pop_back();

    if (m_stack.empty()) {
        m_multDirty = false;
        m_mult = Math::Matrix();
    }
}
Math::Matrix MatrixStack::mult()
{
    if (m_multDirty) {
        Math::Matrix m;
        auto iter = m_stack.begin();
        while (iter != m_stack.end()) {
            m = (*iter) * m;
            ++iter;
        }
        m_mult = m;
        m_multDirty = false;
    }
    return m_mult;
}
void MatrixStack::clear()
{
    m_stack.clear();
    m_multDirty = false;
    m_mult = Math::Matrix();
}
}