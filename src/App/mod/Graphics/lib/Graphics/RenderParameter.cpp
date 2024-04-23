#include <Graphics/Internal/Constant.hpp>
#include <Graphics/RenderParameter.hpp>
#include <wrl/client.h>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<RenderParameter> RenderParameter::create(RenderInterface interfaze)
{
    auto param = std::shared_ptr<RenderParameter>(new RenderParameter());
    param->m_constant = Internal::Constant::create(interfaze);
    return param;
}
RenderParameter::~RenderParameter() { }
void RenderParameter::update() { m_constant->update(); }

void RenderParameter::setTransform(const Math::Matrix& transform) { m_constant->setTransform(transform); }
Math::Matrix RenderParameter::getTransform() const { return m_constant->getTransform(); }

void RenderParameter::setTexture(const std::shared_ptr<Texture>& texture) { m_constant->setTexture(texture); }
std::shared_ptr<Texture> RenderParameter::getTexture() const { return m_constant->getTexture(); }

void RenderParameter::setColor(const Math::Vector4& color) { m_constant->setColor(color); }
Math::Vector4 RenderParameter::getColor() const { return m_constant->getColor(); }

RenderInterface RenderParameter::getInterface() const { return m_constant->getInterface(); }
// private
RenderParameter::RenderParameter()
    : m_constant(nullptr)
{
}
std::shared_ptr<Internal::Constant> RenderParameter::getConstant() const { return m_constant; }
}