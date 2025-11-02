#pragma once
#include <Graphics/Metadata.hpp>
#include <d3d12.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class UniformBuffer;
/**
 * ユニフォームのプールです。
 * DirectX12ではユニフォームを複数の描画に使いまわすことができない。
 * 例えばあるオブジェクトを異なる色や位置で複数描くときに一つのユニフォームを書き換えて再利用はできない。
 * ので、このプールを用いてそれぞれに個別のユニフォームを割り当てる。
 */
class UniformPool {
public:
    /**
     * プログラムに対応したユニフォームをプールから取得します。
     * @param entry
     * @return
     */
    static std::shared_ptr<UniformBuffer> rent(Metadata::ProgramTable entry);

    /**
     * ユニフォームをプールへ返します。
     * @param ub
     */
    static void release(const std::shared_ptr<UniformBuffer>& ub);

    /**
     * ユニフォームを全てプールへ返します。
     */
    static void releaseAll();

#if SOLID_ENABLE_INTERNAL
    static void initialize();
    static void destroy();
#endif
private:
    UniformPool() = delete;
    static uint32_t progressBufferOffset(Metadata::ProgramTable entry);

    static const int32_t k_bufferUnitSize;
    static Microsoft::WRL::ComPtr<ID3D12Resource> s_globalBuffer;
    static void* s_globalBufferMemory;
    static uint64_t s_globalBufferOffset;

    static std::vector<std::vector<std::shared_ptr<UniformBuffer>>> s_freeTable;
    static std::vector<std::vector<std::shared_ptr<UniformBuffer>>> s_usedTable;
};
}