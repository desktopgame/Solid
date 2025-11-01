#pragma once
#include <Graphics/Device.hpp>
#include <Graphics/IBuffer.hpp>
#include <Math/Vector.hpp>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <thread>
#include <vector>
#include <wrl/client.h>

namespace Lib::Graphics {
class Device;
class Constant;
class Shader;
class Swapchain;
class TileBatch;
class RenderContext;
class UniformBuffer;
class DualBuffer;
class GpuBuffer;
class Texture;
/**
 * 描画パイプラインを管理するクラスです。
 */
class Surface {
public:
    ~Surface();

    /**
     * ImGUIの描画を開始します。
     */
    void beginGui();

    /**
     * ImGUIの描画を終了します。
     */
    void endGui();

    /**
     * 3Dの描画を開始します。
     */
    void begin3D();

    /**
     * 3Dの描画を終了します。
     */
    void end3D();

    /**
     * 2Dの描画を開始します。
     */
    void begin2D();

    /**
     * 2Dの描画を終了します。
     */
    void end2D();

    /**
     * この呼び出しに続く描画が頂点情報を共有できることをマークします。
     * この場合には、少しだけ命令を省略します。
     * @param rc
     */
    void beginBatch(const std::shared_ptr<RenderContext>& rc);

    /**
     * 頂点情報の共有を終了します。
     * @param rc
     */
    void endBatch(const std::shared_ptr<RenderContext>& rc);

    /**
     * プレゼンテーションを開始します。
     */
    void beginPresent();

    /**
     * コマンドの終了を待機します。
     */
    void endPresent();

    /**
     * バッファの中身を同期します。
     * @param dualBuffer
     */
    void sync(const std::shared_ptr<DualBuffer>& dualBuffer);

    /**
     * ステンシル参照値を設定します。
     * @param value
     */
    void stencilRef(uint32_t value);

    /**
     * ステンシルをクリアします。
     */
    void stencilClear();

    /**
     * 頂点シェーダのユニフォームを設定します。
     * @param ub
     * @param index
     * @param data
     */
    void uniformVS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data);

    /**
     * ジオメトリシェーダのユニフォームを設定します。
     * @param ub
     * @param index
     * @param data
     */
    void uniformGS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data);

    /**
     * ピクセルシェーダーのユニフォームを設定します。
     * @param ub
     * @param index
     * @param data
     */
    void uniformPS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data);

    /**
     * ピクセルシェーダーのユニフォームを設定します。
     * @param ub
     * @param index
     * @param texture
     */
    void uniformPS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const std::shared_ptr<Texture>& texture);

    /**
     * コンピュートシェーダーのユニフォームを設定します。
     * @param ub
     * @param index
     * @param data
     */
    void uniformCS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data);

    /**
     * コンピュートシェーダーのユニフォームを設定します。
     * @param ub
     * @param index
     * @param texture
     */
    void uniformCS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const std::shared_ptr<Texture>& texture);

    /**
     * コンピュートシェーダーのユニフォームを設定します。
     * @param ub
     * @param index
     * @param buffer
     */
    void uniformCS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const std::shared_ptr<GpuBuffer>& buffer);

    /**
     * 指定のレンダーコンテキストで描画します。
     * @param rc
     * @param ub
     * @param vertex
     * @param index
     * @param indexLength
     */
    void render(
        const std::shared_ptr<RenderContext>& rc,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<IBuffer>& vertex,
        const std::shared_ptr<IBuffer>& index,
        int32_t indexLength);

    /**
     * 指定のレンダーコンテキストでインスタンスを描画します。
     * @param rc
     * @param ub
     * @param vertex
     * @param index
     * @param indexLength
     */
    void render(
        const std::shared_ptr<RenderContext>& rc,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<IBuffer>& vertex,
        const std::shared_ptr<IBuffer>& index,
        int32_t indexLength,
        const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
        int32_t instanceCount);

    /**
     * 指定のレンダーコンテキストでインスタンスを描画します。
     * @param rc
     * @param ub
     * @param vertex
     * @param index
     * @param indexLength
     */
    void render(
        const std::shared_ptr<RenderContext>& rc,
        const std::shared_ptr<UniformBuffer>& ub,
        const std::shared_ptr<IBuffer>& vertex,
        const std::shared_ptr<IBuffer>& index,
        int32_t indexLength,
        const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
        int32_t instanceCount,
        int32_t threadGroupCountX,
        int32_t threadGroupCountY,
        int32_t threadGroupCountZ);

#if SOLID_ENABLE_INTERNAL
    static std::shared_ptr<Surface> create(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::shared_ptr<Swapchain>& swapchain);
    void destroy();
#endif

private:
    Surface();
    void init(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const std::shared_ptr<Swapchain>& swapchain);

    void bloomWrite(int32_t index);
    void bloomRead(int32_t index);
    void threadRun();
    void threadExit();

    class Impl;
    std::shared_ptr<Impl> m_impl;

    class ICommand;
    class BeginGuiCommand;
    class EndGuiCommand;
    class Begin3DCommand;
    class End3DCommand;
    class Begin2DCommand;
    class End2DCommand;
    class BeginBatchCommand;
    class EndBatchCommand;
    class PresentCommand;
    class SyncCommand;
    class StencilRefCommand;
    class StencilClearCommand;
    class RenderCommand1;
    class RenderCommand2;
    class RenderCommand3;
    class UniformVSCommand;
    class UniformGSCommand;
    class UniformPSCommand;
    class UniformCSCommand;
    class WaitCommand;
    class ExitCommand;

    std::unique_ptr<std::thread> m_thread;
    bool m_threadRunning;
    std::mutex m_threadSyncMutex;
    std::condition_variable m_threadSyncCondVar;
    bool m_threadSyncFlag;
    std::shared_ptr<Swapchain> m_swapchain;

    Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_infoQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_gHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_gTextures;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_bloomHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_bloomTextures;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthBuffer;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_depthStencilViewHeap;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    bool m_sendDrawCall;
};
}