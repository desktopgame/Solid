#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <memory>

namespace Lib::Graphics {
class ITileBuffer {
public:
    virtual ~ITileBuffer() = 0;

    virtual Math::Vector4* getArrayAt(int32_t index) = 0;
    virtual Math::Matrix& getMatrixAt(int32_t index) = 0;
    virtual size_t getArraySize() const = 0;

    virtual const void* getElementPtr() const = 0;
    virtual size_t getElementSize() const = 0;
    virtual size_t getElementCount() const = 0;
};

class TileBufferUltraSmall : public ITileBuffer {
public:
    struct Memory {
    public:
        std::array<Math::Vector4, 28> tiles; // 448byte
        Math::Matrix matrix; // 64byte
    };

    static std::shared_ptr<TileBufferUltraSmall> create(int32_t elementCount);
    virtual ~TileBufferUltraSmall() = default;

    Math::Vector4* getArrayAt(int32_t index) override;
    Math::Matrix& getMatrixAt(int32_t index) override;
    size_t getArraySize() const override;

    const void* getElementPtr() const override;
    size_t getElementSize() const override;
    size_t getElementCount() const override;

private:
    explicit TileBufferUltraSmall(int32_t elementCount);
    std::vector<Memory> m_memoryVec;
};
}