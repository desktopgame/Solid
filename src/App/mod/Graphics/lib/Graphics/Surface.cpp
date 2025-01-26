#include <Graphics/BloomEffect.hpp>
#include <Graphics/CpuBuffer.hpp>
#include <Graphics/Device.hpp>
#include <Graphics/DualBuffer.hpp>
#include <Graphics/GlobalLight.hpp>
#include <Graphics/PointLight.hpp>
#include <Graphics/RenderContext.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Surface.hpp>
#include <Graphics/Swapchain.hpp>
#include <Graphics/UniformBuffer.hpp>
#include <Graphics/UniformPool.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Graphics/VertexTexCoord3D.hpp>
#include <Math/Vector.hpp>
#include <Utils/LockfreeQueue.hpp>
#include <Utils/Stopwatch.hpp>
#include <Utils/String.hpp>
#include <stdexcept>
#include <vector>

namespace Lib::Graphics {

using Microsoft::WRL::ComPtr;
// Command
class Surface::ICommand {
public:
    explicit ICommand() = default;
    virtual ~ICommand() = default;
    virtual void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) = 0;

private:
};

class Surface::BeginGuiCommand : public ICommand {
public:
    explicit BeginGuiCommand()
        : swapchain(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        // これはメインスレッドで呼ぶのが正しい
        // ImGui::Renderさえ描画スレッドで呼べばいい
        // swapchain->guiClear();
    }

    std::shared_ptr<Swapchain> swapchain;
};

class Surface::EndGuiCommand : public ICommand {
public:
    explicit EndGuiCommand()
        : swapchain(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        swapchain->guiRender();
    }

    std::shared_ptr<Swapchain> swapchain;
};

class Surface::Begin3DCommand : public ICommand {
public:
    explicit Begin3DCommand()
        : surface(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        auto d3d12Device = Engine::getInstance()->getDevice()->getID3D12Device();
        // Barrier
        D3D12_RESOURCE_BARRIER barriers[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_RESOURCE_BARRIER& barrier = barriers[i];
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Transition.pResource = surface->m_gTextures.at(i).Get();
            barrier.Transition.Subresource = 0;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        }
        commandList->ResourceBarrier(3, barriers);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = surface->m_gHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = surface->m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
        commandList->OMSetRenderTargets(3, &rtvHandle, true, &dsvHandle);

        float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        for (int32_t i = 0; i < 3; i++) {
            commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
            rtvHandle.ptr += d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        D3D12_CLEAR_FLAGS clearDepthFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
        commandList->ClearDepthStencilView(dsvHandle, clearDepthFlags, 1.0f, 0, 0, nullptr);
        commandList->OMSetStencilRef(1);

        // viewport
        D3D12_VIEWPORT viewports[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_VIEWPORT& viewport = viewports[i];
            viewport.Width = Screen::getWidth();
            viewport.Height = Screen::getHeight();
            viewport.TopLeftX = 0;
            viewport.TopLeftY = 0;
            viewport.MaxDepth = 1.0f;
            viewport.MinDepth = 0.0f;
        }
        commandList->RSSetViewports(3, viewports);
        // scissor
        D3D12_RECT scissorRects[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_RECT& scissorRect = scissorRects[i];
            scissorRect.top = 0;
            scissorRect.left = 0;
            scissorRect.right = scissorRect.left + Screen::getWidth();
            scissorRect.bottom = scissorRect.top + Screen::getHeight();
        }
        commandList->RSSetScissorRects(3, scissorRects);
    }

    Surface* surface;
};

class Surface::End3DCommand : public ICommand {
public:
    explicit End3DCommand()
        : surface(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        // Barrier
        D3D12_RESOURCE_BARRIER barriers[3] = {};
        for (int32_t i = 0; i < 3; i++) {
            D3D12_RESOURCE_BARRIER& barrier = barriers[i];
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Transition.pResource = surface->m_gTextures.at(i).Get();
            barrier.Transition.Subresource = 0;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        }
        commandList->ResourceBarrier(3, barriers);

        commandList->OMSetStencilRef(2);
    }

    Surface* surface;
};

class Surface::Begin2DCommand : public ICommand {
public:
    explicit Begin2DCommand()
        : surface(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        /*
        m_swapchain->clear(m_commandList, m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
        GlobalLight::draw(m_commandList);
        PointLight::draw(m_commandList);
        */
        //*
        auto d3d12Device = Engine::getInstance()->getDevice()->getID3D12Device();
        // Barrier
        // Bloom0番テクスチャにGBufferをソースとして書く
        surface->bloomWrite(0);
        GlobalLight::draw(commandList);
        PointLight::draw(commandList);
        surface->bloomRead(0);

        //
        // Bloom0番をソースとしてBloom1番に高輝度成分を書き込む
        //
        surface->bloomWrite(1);
        BloomEffect::drawFilter(commandList);
        surface->bloomRead(1);

        for (int32_t i = 0; i < 8; i++) {
            //
            // Bloom1番をソースとしてBloom2番にブラー書き込む
            //
            surface->bloomWrite(2);
            BloomEffect::drawBlur1(commandList);
            surface->bloomRead(2);

            //
            // Bloom2番をソースとしてBloom1番にブラーを書き込む
            //

            surface->bloomWrite(1);
            BloomEffect::drawBlur2(commandList);
            surface->bloomRead(1);
        }
        //
        // ダイレクトターゲットに戻す
        //
        surface->m_swapchain->clear(commandList, surface->m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
        BloomEffect::drawMix(commandList);
        //*/
    }

    Surface* surface;
};

class Surface::End2DCommand : public ICommand {
public:
    explicit End2DCommand()
        : surface(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        surface->m_swapchain->swap(commandList);
        commandList->Close();
        surface->m_swapchain->execute(commandList);
    }

    Surface* surface;
};

class Surface::BeginBatchCommand : public ICommand {
public:
    explicit BeginBatchCommand()
        : renderContext(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        renderContext->beginBatch();
    }
    std::shared_ptr<RenderContext> renderContext;
};

class Surface::EndBatchCommand : public ICommand {
public:
    explicit EndBatchCommand()
        : renderContext(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        renderContext->endBatch();
    }
    std::shared_ptr<RenderContext> renderContext;
};

class Surface::PresentCommand : public ICommand {
public:
    explicit PresentCommand()
        : surface(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        surface->m_swapchain->present();
        surface->m_swapchain->signal();
    }

    Surface* surface;
};

class Surface::SyncCommand : public ICommand {
public:
    explicit SyncCommand()
        : dualBuffer(nullptr)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        dualBuffer->sync(commandList);
    }

    std::shared_ptr<DualBuffer> dualBuffer;
};

class Surface::RenderCommand1 : public ICommand {
public:
    explicit RenderCommand1()
        : renderContext(nullptr)
        , uniformBuffer(nullptr)
        , vertex(nullptr)
        , index(nullptr)
        , indexLength(0)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        renderContext->render(commandList, uniformBuffer, vertex, index, indexLength);
    }

    std::shared_ptr<RenderContext> renderContext;
    std::shared_ptr<UniformBuffer> uniformBuffer;
    std::shared_ptr<IBuffer> vertex;
    std::shared_ptr<IBuffer> index;
    int32_t indexLength;
};

class Surface::RenderCommand2 : public ICommand {
public:
    explicit RenderCommand2()
        : renderContext(nullptr)
        , uniformBuffer(nullptr)
        , vertex(nullptr)
        , index(nullptr)
        , indexLength(0)
        , instanceBuffers()
        , instanceCount(0)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        renderContext->render(commandList, uniformBuffer, vertex, index, indexLength, instanceBuffers, instanceCount);
    }

    std::shared_ptr<RenderContext> renderContext;
    std::shared_ptr<UniformBuffer> uniformBuffer;
    std::shared_ptr<IBuffer> vertex;
    std::shared_ptr<IBuffer> index;
    int32_t indexLength;
    std::vector<std::shared_ptr<IBuffer>> instanceBuffers;
    int32_t instanceCount;
};

class Surface::RenderCommand3 : public ICommand {
public:
    explicit RenderCommand3()
        : renderContext(nullptr)
        , uniformBuffer(nullptr)
        , vertex(nullptr)
        , index(nullptr)
        , indexLength(0)
        , instanceBuffers()
        , instanceCount(0)
        , threadGroupCountX(0)
        , threadGroupCountY(0)
        , threadGroupCountZ(0)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        uniformBuffer->beginCompute(commandList);
        renderContext->compute(commandList, uniformBuffer, threadGroupCountX, threadGroupCountY, threadGroupCountZ);
        uniformBuffer->syncCompute(commandList);
        uniformBuffer->endCompute(commandList);

        renderContext->render(commandList, uniformBuffer, vertex, index, indexLength, instanceBuffers, instanceCount);
    }

    std::shared_ptr<RenderContext> renderContext;
    std::shared_ptr<UniformBuffer> uniformBuffer;
    std::shared_ptr<IBuffer> vertex;
    std::shared_ptr<IBuffer> index;
    int32_t indexLength;
    std::vector<std::shared_ptr<IBuffer>> instanceBuffers;
    int32_t instanceCount;
    int32_t threadGroupCountX;
    int32_t threadGroupCountY;
    int32_t threadGroupCountZ;
};

class Surface::UniformVSCommand : public ICommand {
public:
    explicit UniformVSCommand() { }
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        uniformBuffer->setVS(index, vram);
    }
    std::shared_ptr<UniformBuffer> uniformBuffer;
    int32_t index;
    void* vram;
};

class Surface::UniformGSCommand : public ICommand {
public:
    explicit UniformGSCommand() { }
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        uniformBuffer->setGS(index, vram);
    }
    std::shared_ptr<UniformBuffer> uniformBuffer;
    int32_t index;
    void* vram;
};

class Surface::UniformPSCommand : public ICommand {
public:
    explicit UniformPSCommand() { }
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        if (texture) {
            uniformBuffer->setPS(index, texture);
        } else {
            uniformBuffer->setPS(index, vram);
        }
    }
    std::shared_ptr<UniformBuffer> uniformBuffer;
    int32_t index;
    void* vram;
    std::shared_ptr<Texture> texture;
};

class Surface::UniformCSCommand : public ICommand {
public:
    explicit UniformCSCommand() { }
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        if (texture) {
            uniformBuffer->setCS(index, texture);
        } else if (gpuBuffer) {
            uniformBuffer->setCS(index, gpuBuffer);
        } else {
            uniformBuffer->setCS(index, vram);
        }
    }
    std::shared_ptr<UniformBuffer> uniformBuffer;
    int32_t index;
    void* vram;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<GpuBuffer> gpuBuffer;
};

class Surface::WaitCommand : public ICommand {
public:
    explicit WaitCommand()
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        std::unique_lock<std::mutex> lock(surface->m_threadSyncMutex);
        surface->m_threadSyncFlag = true;
        surface->m_threadSyncCondVar.notify_one();
    }

    Surface* surface;
};

class Surface::ExitCommand : public ICommand {
public:
    explicit ExitCommand()
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        surface->m_threadRunning = false;
    }

    Surface* surface;
};

template <typename T>
class CommandPool {
public:
    explicit CommandPool()
        : m_usedVec()
        , m_freeVec()
    {
    }

    ~CommandPool()
    {
        // NOTE: 参照カウントが無視できないコストなのでスマポは使わない
        for (auto ptr : m_usedVec) {
            delete ptr;
        }
        m_usedVec.clear();

        for (auto ptr : m_freeVec) {
            delete ptr;
        }
        m_freeVec.clear();
    }

    T* rent()
    {
        if (m_freeVec.size() > 0) {
            T* ub = m_freeVec.back();
            m_freeVec.pop_back();
            m_usedVec.emplace_back(ub);
            return ub;
        } else {
            T* ub = new T();
            m_usedVec.emplace_back(ub);
            return ub;
        }
        return nullptr;
    }

    void releaseAll()
    {
        for (const auto& s : m_usedVec) {
            m_freeVec.emplace_back(s);
        }

        m_usedVec.clear();
    }

private:
    std::vector<T*> m_usedVec;
    std::vector<T*> m_freeVec;
};
// Impl
class Surface::Impl {
public:
    explicit Impl()
        : queue(8192)
    {
    }
    Utils::LockfreeQueue<ICommand*> queue;
    CommandPool<BeginGuiCommand> beginGuiCommandPool;
    CommandPool<EndGuiCommand> endGuiCommandPool;
    CommandPool<Begin3DCommand> begin3DCommandPool;
    CommandPool<End3DCommand> end3DCommandPool;
    CommandPool<Begin2DCommand> begin2DCommandPool;
    CommandPool<End2DCommand> end2DCommandPool;
    CommandPool<BeginBatchCommand> beginBatchCommandPool;
    CommandPool<EndBatchCommand> endBatchCommandPool;
    CommandPool<PresentCommand> presentCommandPool;
    CommandPool<SyncCommand> syncCommandPool;
    CommandPool<RenderCommand1> renderCommand1Pool;
    CommandPool<RenderCommand2> renderCommand2Pool;
    CommandPool<RenderCommand3> renderCommand3Pool;
    CommandPool<UniformVSCommand> uniformVSPool;
    CommandPool<UniformGSCommand> uniformGSPool;
    CommandPool<UniformPSCommand> uniformPSPool;
    CommandPool<UniformCSCommand> uniformCSPool;
    CommandPool<WaitCommand> waitPool;
    CommandPool<ExitCommand> exitPool;
};
// public
Surface::~Surface()
{
}

void Surface::beginGui()
{
    m_swapchain->guiClear();
}

void Surface::endGui()
{
    auto cmd = m_impl->endGuiCommandPool.rent();
    cmd->swapchain = m_swapchain;
    m_impl->queue.enqueue(cmd);
}

void Surface::begin3D()
{
    auto cmd = m_impl->begin3DCommandPool.rent();
    cmd->surface = this;
    m_impl->queue.enqueue(cmd);
}

void Surface::end3D()
{
    auto cmd = m_impl->end3DCommandPool.rent();
    cmd->surface = this;
    m_impl->queue.enqueue(cmd);
}

void Surface::begin2D()
{
    auto cmd = m_impl->begin2DCommandPool.rent();
    cmd->surface = this;
    m_impl->queue.enqueue(cmd);
}

void Surface::end2D()
{
    auto cmd = m_impl->end2DCommandPool.rent();
    cmd->surface = this;
    m_impl->queue.enqueue(cmd);
}

void Surface::beginBatch(const std::shared_ptr<RenderContext>& rc)
{
    auto cmd = m_impl->beginBatchCommandPool.rent();
    cmd->renderContext = rc;
    m_impl->queue.enqueue(cmd);
}

void Surface::endBatch(const std::shared_ptr<RenderContext>& rc)
{
    auto cmd = m_impl->endBatchCommandPool.rent();
    cmd->renderContext = rc;
    m_impl->queue.enqueue(cmd);
}

void Surface::beginPresent()
{
    m_swapchain->fence();
    {
        auto cmd = m_impl->presentCommandPool.rent();
        cmd->surface = this;
        m_impl->queue.enqueue(cmd);
    }
}

void Surface::endPresent()
{
    m_swapchain->waitSync();

    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator.Get(), nullptr);
    UniformPool::releaseAll();

    m_impl->beginGuiCommandPool.releaseAll();
    m_impl->endGuiCommandPool.releaseAll();
    m_impl->begin3DCommandPool.releaseAll();
    m_impl->end3DCommandPool.releaseAll();
    m_impl->begin2DCommandPool.releaseAll();
    m_impl->end2DCommandPool.releaseAll();
    m_impl->beginBatchCommandPool.releaseAll();
    m_impl->endBatchCommandPool.releaseAll();
    m_impl->presentCommandPool.releaseAll();
    m_impl->syncCommandPool.releaseAll();
    m_impl->renderCommand1Pool.releaseAll();
    m_impl->renderCommand2Pool.releaseAll();
    m_impl->renderCommand3Pool.releaseAll();
    m_impl->uniformVSPool.releaseAll();
    m_impl->uniformGSPool.releaseAll();
    m_impl->uniformPSPool.releaseAll();
    m_impl->uniformCSPool.releaseAll();

    m_vramOffset = 0;
}

void Surface::sync(const std::shared_ptr<DualBuffer>& dualBuffer)
{
    auto cmd = m_impl->syncCommandPool.rent();
    cmd->dualBuffer = dualBuffer;
    m_impl->queue.enqueue(cmd);
}

void Surface::uniformVS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data)
{
    const Metadata::Program& program = Metadata::k_programs.at(ub->getEntry());
    size_t size = program.vsUniforms.at(index).size;
    void* memory = (unsigned char*)m_vram + m_vramOffset;
    ::memcpy(memory, data, size);
    m_vramOffset += size;

    auto cmd = m_impl->uniformVSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = memory;
    m_impl->queue.enqueue(cmd);
}
void Surface::uniformGS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data)
{
    const Metadata::Program& program = Metadata::k_programs.at(ub->getEntry());
    size_t size = program.gsUniforms.at(index).size;
    void* memory = (unsigned char*)m_vram + m_vramOffset;
    ::memcpy(memory, data, size);
    m_vramOffset += size;

    auto cmd = m_impl->uniformGSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = memory;
    m_impl->queue.enqueue(cmd);
}
void Surface::uniformPS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data)
{
    const Metadata::Program& program = Metadata::k_programs.at(ub->getEntry());
    size_t size = program.psUniforms.at(index).size;
    void* memory = (unsigned char*)m_vram + m_vramOffset;
    ::memcpy(memory, data, size);
    m_vramOffset += size;

    auto cmd = m_impl->uniformPSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = memory;
    cmd->texture = nullptr;
    m_impl->queue.enqueue(cmd);
}
void Surface::uniformPS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const std::shared_ptr<Texture>& texture)
{
    auto cmd = m_impl->uniformPSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = nullptr;
    cmd->texture = texture;
    m_impl->queue.enqueue(cmd);
}
void Surface::uniformCS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const void* data)
{
    const Metadata::Program& program = Metadata::k_programs.at(ub->getEntry());
    size_t size = program.csUniforms.at(index).size;
    void* memory = (unsigned char*)m_vram + m_vramOffset;
    ::memcpy(memory, data, size);
    m_vramOffset += size;

    auto cmd = m_impl->uniformCSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = memory;
    cmd->gpuBuffer = nullptr;
    cmd->texture = nullptr;
    m_impl->queue.enqueue(cmd);
}
void Surface::uniformCS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const std::shared_ptr<Texture>& texture)
{
    auto cmd = m_impl->uniformCSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = nullptr;
    cmd->gpuBuffer = nullptr;
    cmd->texture = texture;
    m_impl->queue.enqueue(cmd);
}
void Surface::uniformCS(const std::shared_ptr<UniformBuffer>& ub, int32_t index, const std::shared_ptr<GpuBuffer>& buffer)
{
    auto cmd = m_impl->uniformCSPool.rent();
    cmd->uniformBuffer = ub;
    cmd->index = index;
    cmd->vram = nullptr;
    cmd->gpuBuffer = buffer;
    cmd->texture = nullptr;
    m_impl->queue.enqueue(cmd);
}

void Surface::render(
    const std::shared_ptr<RenderContext>& rc,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<IBuffer>& vertex,
    const std::shared_ptr<IBuffer>& index,
    int32_t indexLength)
{
    auto cmd = m_impl->renderCommand1Pool.rent();
    cmd->renderContext = rc;
    cmd->uniformBuffer = ub;
    cmd->vertex = vertex;
    cmd->index = index;
    cmd->indexLength = indexLength;
    m_impl->queue.enqueue(cmd);
}

void Surface::render(
    const std::shared_ptr<RenderContext>& rc,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<IBuffer>& vertex,
    const std::shared_ptr<IBuffer>& index,
    int32_t indexLength,
    const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
    int32_t instanceCount)
{
    auto cmd = m_impl->renderCommand2Pool.rent();
    cmd->renderContext = rc;
    cmd->uniformBuffer = ub;
    cmd->vertex = vertex;
    cmd->index = index;
    cmd->indexLength = indexLength;
    cmd->instanceBuffers = instanceBuffers;
    cmd->instanceCount = instanceCount;
    m_impl->queue.enqueue(cmd);
}

void Surface::render(
    const std::shared_ptr<RenderContext>& rc,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<IBuffer>& vertex,
    const std::shared_ptr<IBuffer>& index,
    int32_t indexLength,
    const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
    int32_t instanceCount,
    int32_t threadGroupCountX,
    int32_t threadGroupCountY,
    int32_t threadGroupCountZ)
{
    auto cmd = m_impl->renderCommand3Pool.rent();
    cmd->renderContext = rc;
    cmd->uniformBuffer = ub;
    cmd->vertex = vertex;
    cmd->index = index;
    cmd->indexLength = indexLength;
    cmd->instanceBuffers = instanceBuffers;
    cmd->instanceCount = instanceCount;
    cmd->threadGroupCountX = threadGroupCountX;
    cmd->threadGroupCountY = threadGroupCountY;
    cmd->threadGroupCountZ = threadGroupCountZ;
    m_impl->queue.enqueue(cmd);
}

// internal
std::shared_ptr<Surface> Surface::create(
    const Microsoft::WRL::ComPtr<ID3D12Device>& device,
    const std::shared_ptr<Swapchain>& swapchain)
{
    auto surface = std::shared_ptr<Surface>(new Surface());
    surface->init(device, swapchain);
    return surface;
}

void Surface::destroy()
{
    auto cmd = m_impl->waitPool.rent();
    cmd->surface = this;
    m_impl->queue.enqueue(cmd);

    {
        std::unique_lock<std::mutex> lock(m_threadSyncMutex);
        m_threadSyncCondVar.wait(lock, [&]() -> bool { return m_threadSyncFlag; });
    }

    m_swapchain->fence();
    m_swapchain->present();
    m_swapchain->signal();
    m_swapchain->waitSync();

    threadExit();
    m_impl = nullptr;

    GlobalLight::destroy();
    PointLight::destroy();
    BloomEffect::destroy();
    m_swapchain->destroy();
    m_swapchain = nullptr;
    ::free(m_vram);
    m_vram = nullptr;
}
// private
Surface::Surface()
    : m_impl()
    , m_thread()
    , m_threadRunning(true)
    , m_threadSyncMutex()
    , m_threadSyncCondVar()
    , m_threadSyncFlag(false)
    , m_swapchain()
    , m_dxgiFactory()
    , m_infoQueue()
    , m_commandAllocator()
    , m_commandList()
    , m_gHeap()
    , m_gTextures(3)
    , m_bloomHeap()
    , m_bloomTextures(3)
    , m_depthBuffer()
    , m_depthStencilViewHeap()
    , m_fence()
    , m_vram(::malloc(::pow(2, 20) * 512))
    , m_vramOffset(0)
{
}

void Surface::init(
    const Microsoft::WRL::ComPtr<ID3D12Device>& device,
    const std::shared_ptr<Swapchain>& swapchain)
{
    m_swapchain = swapchain;
    // CommandAllocator
    if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)))) {
        throw std::runtime_error("failed CreateCommandAllocator()");
    }
    // CommandList
    if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)))) {
        throw std::runtime_error("failed CreateCommandList()");
    }
    // GBuffer
    D3D12_DESCRIPTOR_HEAP_DESC gHeapDesc = {};
    gHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    gHeapDesc.NodeMask = 0;
    gHeapDesc.NumDescriptors = 3;
    gHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(device->CreateDescriptorHeap(&gHeapDesc, IID_PPV_ARGS(&m_gHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    D3D12_CPU_DESCRIPTOR_HANDLE gHandle = m_gHeap->GetCPUDescriptorHandleForHeapStart();
    for (int32_t i = 0; i < 3; i++) {
        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        clearValue.Color[0] = 0.0f;
        clearValue.Color[1] = 0.0f;
        clearValue.Color[2] = 0.0f;
        clearValue.Color[3] = 0.0f;

        D3D12_HEAP_PROPERTIES texHeapProps = {};
        texHeapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
        texHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        texHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        texHeapProps.CreationNodeMask = 0;
        texHeapProps.VisibleNodeMask = 0;
        D3D12_RESOURCE_DESC texResDesc = {};
        texResDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        texResDesc.Width = Screen::getWidth();
        texResDesc.Height = Screen::getHeight();
        texResDesc.DepthOrArraySize = 1;
        texResDesc.SampleDesc.Count = 1;
        texResDesc.SampleDesc.Quality = 0;
        texResDesc.MipLevels = 1;
        texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        texResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        ComPtr<ID3D12Resource>& gTexture = m_gTextures.at(i);
        if (FAILED(device->CreateCommittedResource(&texHeapProps, D3D12_HEAP_FLAG_NONE, &texResDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&gTexture)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }

        switch (i) {
        case 0:
            gTexture->SetName(L"Position");
            break;
        case 1:
            gTexture->SetName(L"Normal");
            break;
        case 2:
            gTexture->SetName(L"Color");
            break;
        }

        D3D12_RENDER_TARGET_VIEW_DESC gRtvDesc = {};
        gRtvDesc.Texture2D.MipSlice = 0;
        gRtvDesc.Texture2D.PlaneSlice = 0;
        gRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        gRtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        device->CreateRenderTargetView(gTexture.Get(), nullptr, gHandle);
        gHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    // Bloom
    D3D12_DESCRIPTOR_HEAP_DESC bloomHeapDesc = {};
    bloomHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    bloomHeapDesc.NodeMask = 0;
    bloomHeapDesc.NumDescriptors = 3;
    bloomHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(device->CreateDescriptorHeap(&bloomHeapDesc, IID_PPV_ARGS(&m_bloomHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    D3D12_CPU_DESCRIPTOR_HANDLE bloomHandle = m_bloomHeap->GetCPUDescriptorHandleForHeapStart();
    for (int32_t i = 0; i < 3; i++) {
        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        clearValue.Color[0] = 0.0f;
        clearValue.Color[1] = 0.0f;
        clearValue.Color[2] = 0.0f;
        clearValue.Color[3] = 0.0f;

        D3D12_HEAP_PROPERTIES texHeapProps = {};
        texHeapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
        texHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        texHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        texHeapProps.CreationNodeMask = 0;
        texHeapProps.VisibleNodeMask = 0;
        D3D12_RESOURCE_DESC texResDesc = {};
        texResDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        if (i == 0) {
            texResDesc.Width = Screen::getWidth();
            texResDesc.Height = Screen::getHeight();
        } else {
            texResDesc.Width = Screen::getWidth() / 4;
            texResDesc.Height = Screen::getHeight() / 4;
        }
        texResDesc.DepthOrArraySize = 1;
        texResDesc.SampleDesc.Count = 1;
        texResDesc.SampleDesc.Quality = 0;
        texResDesc.MipLevels = 1;
        texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        texResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        ComPtr<ID3D12Resource>& bTexture = m_bloomTextures.at(i);
        if (FAILED(device->CreateCommittedResource(&texHeapProps, D3D12_HEAP_FLAG_NONE, &texResDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&bTexture)))) {
            throw std::runtime_error("failed CreateCommittedResource()");
        }

        char buf[16];
        ::memset(buf, '\0', 16);
        ::sprintf(buf, "Bloom[%d]", i);
        bTexture->SetName(Utils::String::toWideString(buf).c_str());

        D3D12_RENDER_TARGET_VIEW_DESC bloomRtvDesc = {};
        bloomRtvDesc.Texture2D.MipSlice = 0;
        bloomRtvDesc.Texture2D.PlaneSlice = 0;
        bloomRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        bloomRtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        device->CreateRenderTargetView(bTexture.Get(), nullptr, bloomHandle);
        bloomHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    // DepthBuffer
    D3D12_RESOURCE_DESC depthResDesc = {};
    depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResDesc.Width = Screen::getWidth();
    depthResDesc.Height = Screen::getHeight();
    depthResDesc.DepthOrArraySize = 1;
    depthResDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthResDesc.SampleDesc.Count = 1;
    depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    D3D12_HEAP_PROPERTIES depthHeapProps = {};
    depthHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    depthHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    depthHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    D3D12_CLEAR_VALUE depthClearValue = {};
    depthClearValue.DepthStencil.Depth = 1.0f;
    depthClearValue.DepthStencil.Stencil = 0;
    depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (FAILED(device->CreateCommittedResource(&depthHeapProps, D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&m_depthBuffer)))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
    m_depthBuffer->SetName(L"DepthBuffer");
    // DepthStencilViewHeap
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    if (FAILED(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_depthStencilViewHeap)))) {
        throw std::runtime_error("failed CreateDescriptorHeap()");
    }
    // DepthBufferView
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
    // GlobalLight
    GlobalLight::initialize(device, m_gTextures);
    // PointLight
    PointLight::initialize(device, m_gTextures);
    // Bloom
    BloomEffect::initialize(device, m_bloomTextures);
    // Thread
    m_impl = std::make_shared<Impl>();
    m_thread = std::make_unique<std::thread>(&Surface::threadRun, this);
}

void Surface::bloomWrite(int32_t index)
{
    auto d3d12Device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_bloomTextures.at(index).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_commandList->ResourceBarrier(1, &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_bloomHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += index * d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

    if (index == 0) {
        float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    }

    // viewport
    D3D12_VIEWPORT viewport = {};
    if (index == 0) {
        viewport.Width = Screen::getWidth();
        viewport.Height = Screen::getHeight();
    } else {
        viewport.Width = Screen::getWidth() / 4;
        viewport.Height = Screen::getHeight() / 4;
    }
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    m_commandList->RSSetViewports(1, &viewport);
    // scissor
    D3D12_RECT scissorRect = {};
    scissorRect.top = 0;
    scissorRect.left = 0;
    if (index == 0) {
        scissorRect.right = scissorRect.left + Screen::getWidth();
        scissorRect.bottom = scissorRect.top + Screen::getHeight();
    } else {
        scissorRect.right = scissorRect.left + Screen::getWidth() / 4;
        scissorRect.bottom = scissorRect.top + Screen::getHeight() / 4;
    }
    m_commandList->RSSetScissorRects(1, &scissorRect);
}

void Surface::bloomRead(int32_t index)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_bloomTextures.at(index).Get();
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    m_commandList->ResourceBarrier(1, &barrier);
}

void Surface::threadRun()
{
    while (m_threadRunning) {
        ICommand* cmd;
        if (m_impl->queue.dequeue(cmd)) {
            cmd->execute(m_commandList);
        }
    }
}

void Surface::threadExit()
{
    auto cmd = m_impl->exitPool.rent();
    cmd->surface = this;
    m_impl->queue.enqueue(cmd);
    m_thread->join();
    m_thread = nullptr;
    m_impl->exitPool.releaseAll();
}
}