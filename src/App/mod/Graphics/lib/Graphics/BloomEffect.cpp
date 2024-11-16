#include <Graphics/BloomEffect.hpp>

namespace Lib::Graphics {
// public
// internal

void BloomEffect::draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList)
{
}

void BloomEffect::initialize(
    const Microsoft::WRL::ComPtr<ID3D12Device>& device,
    const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& bloomTextures)
{
}
void BloomEffect::destroy()
{
}
// private
}