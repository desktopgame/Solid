#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace Lib::Graphics {
/**
 * バッファーの共通インターフェイスです。
 */
class IBuffer {
public:
    explicit IBuffer() = default;
    virtual ~IBuffer() = default;

    /**
     * バッファーのサイズを返します。
     * @return
     */
    virtual size_t getSize() const = 0;

#if SOLID_ENABLE_INTERNAL
    virtual Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const = 0;
#endif
};
}