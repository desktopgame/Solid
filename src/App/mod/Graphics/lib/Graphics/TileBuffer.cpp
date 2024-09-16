#include <Graphics/TileBuffer.hpp>

namespace Lib::Graphics {
// TileBuffer
ITileBuffer::~ITileBuffer() { }
// TileBufferUltraSmall
// public
std::shared_ptr<TileBufferUltraSmall> TileBufferUltraSmall::create(int32_t elementCount) { return std::shared_ptr<TileBufferUltraSmall>(new TileBufferUltraSmall(elementCount)); }
Math::Vector4* TileBufferUltraSmall::getArrayAt(int32_t index) { return m_memoryVec.at(index).tiles.data(); }
Math::Matrix& TileBufferUltraSmall::getMatrixAt(int32_t index) { return m_memoryVec.at(index).matrix; }
size_t TileBufferUltraSmall::getArraySize() const { return 28; }
const void* TileBufferUltraSmall::getElementPtr() const { return reinterpret_cast<const void*>(m_memoryVec.data()); }
size_t TileBufferUltraSmall::getElementSize() const { return sizeof(TileBufferUltraSmall::Memory); }
size_t TileBufferUltraSmall::getElementCount() const { return m_memoryVec.size(); }
// private
TileBufferUltraSmall::TileBufferUltraSmall(int32_t elementCount)
    : m_memoryVec(elementCount)
{
}
}