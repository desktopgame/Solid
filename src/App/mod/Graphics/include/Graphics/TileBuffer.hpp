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

template <int32_t N>
class TileBufferN : public ITileBuffer {
public:
    struct Memory {
    public:
        std::array<Math::Vector4, N> tiles;
        Math::Matrix matrix;
    };
    static_assert(sizeof(Memory) % 256 == 0, "not aligned to 256 bytes");

    inline static std::shared_ptr<TileBufferN<N>> create(int32_t elementCount) { return std::shared_ptr<TileBufferN>(new TileBufferN(elementCount)); }
    inline virtual ~TileBufferN() = default;

    inline Math::Vector4* getArrayAt(int32_t index) override { return m_memoryVec.at(index).tiles.data(); }
    inline Math::Matrix& getMatrixAt(int32_t index) override { return m_memoryVec.at(index).matrix; }
    inline size_t getArraySize() const override { return N; }

    inline const void* getElementPtr() const override { return reinterpret_cast<const void*>(m_memoryVec.data()); }
    inline size_t getElementSize() const override { return sizeof(Memory); }
    inline size_t getElementCount() const override { return m_memoryVec.size(); }

private:
    explicit inline TileBufferN(int32_t elementCount)
        : m_memoryVec(elementCount)
    {
    }
    std::vector<Memory> m_memoryVec;
};

enum class TileBufferKind : int32_t {
    UltraSmall = 0,
    Small,
    Medium,
    Large,
    UltraLarge
};

using TileBufferUltraSmall = TileBufferN<16 + 12>;
using TileBufferSmall = TileBufferN<64 + 12>;
using TileBufferMedium = TileBufferN<256 + 12>;
using TileBufferLarge = TileBufferN<1024 + 12>;
using TileBufferUltraLarge = TileBufferN<4064 + 12>;
}