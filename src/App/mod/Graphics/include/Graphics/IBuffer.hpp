#pragma once

namespace Lib::Graphics {
class IBuffer {
public:
    explicit IBuffer() = default;
    virtual ~IBuffer() = default;

    virtual size_t getSize() const = 0;

#if SOLID_ENABLE_INTERNAL
    virtual Microsoft::WRL::ComPtr<ID3D12Resource> getID3D12Resource() const = 0;
#endif
};
}