#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class BloomEffect {
public:
#if SOLID_ENABLE_INTERNAL
    static void draw(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);

    static void initialize(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const Microsoft::WRL::ComPtr<ID3D12Resource>& bloomTextures);
    static void destroy();
#endif

private:
};
}