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
#include <Utils/BlockingQueue.hpp>
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
    explicit BeginGuiCommand(const std::shared_ptr<Swapchain>& swapchain)
        : m_swapchain(swapchain)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_swapchain->guiClear();
    }

private:
    std::shared_ptr<Swapchain> m_swapchain;
};

class Surface::EndGuiCommand : public ICommand {
public:
    explicit EndGuiCommand(const std::shared_ptr<Swapchain>& swapchain)
        : m_swapchain(swapchain)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_swapchain->guiRender();
    }

private:
    std::shared_ptr<Swapchain> m_swapchain;
};

class Surface::Begin3DCommand : public ICommand {
public:
    explicit Begin3DCommand(Surface& surface)
        : m_surface(surface)
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
            barrier.Transition.pResource = m_surface.m_gTextures.at(i).Get();
            barrier.Transition.Subresource = 0;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        }
        commandList->ResourceBarrier(3, barriers);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_surface.m_gHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_surface.m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
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

private:
    Surface& m_surface;
};

class Surface::End3DCommand : public ICommand {
public:
    explicit End3DCommand(Surface& surface)
        : m_surface(surface)
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
            barrier.Transition.pResource = m_surface.m_gTextures.at(i).Get();
            barrier.Transition.Subresource = 0;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        }
        commandList->ResourceBarrier(3, barriers);

        commandList->OMSetStencilRef(2);
    }

private:
    Surface& m_surface;
};

class Surface::Begin2DCommand : public ICommand {
public:
    explicit Begin2DCommand(Surface& surface)
        : m_surface(surface)
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
        m_surface.bloomWrite(0);
        GlobalLight::draw(commandList);
        PointLight::draw(commandList);
        m_surface.bloomRead(0);

        //
        // Bloom0番をソースとしてBloom1番に高輝度成分を書き込む
        //
        m_surface.bloomWrite(1);
        BloomEffect::drawFilter(commandList);
        m_surface.bloomRead(1);

        for (int32_t i = 0; i < 8; i++) {
            //
            // Bloom1番をソースとしてBloom2番にブラー書き込む
            //
            m_surface.bloomWrite(2);
            BloomEffect::drawBlur1(commandList);
            m_surface.bloomRead(2);

            //
            // Bloom2番をソースとしてBloom1番にブラーを書き込む
            //

            m_surface.bloomWrite(1);
            BloomEffect::drawBlur2(commandList);
            m_surface.bloomRead(1);
        }
        //
        // ダイレクトターゲットに戻す
        //
        m_surface.m_swapchain->clear(commandList, m_surface.m_depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
        BloomEffect::drawMix(commandList);
        //*/
    }

private:
    Surface& m_surface;
};

class Surface::End2DCommand : public ICommand {
public:
    explicit End2DCommand(Surface& surface)
        : m_surface(surface)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_surface.m_swapchain->swap(commandList);
        commandList->Close();
        m_surface.m_swapchain->execute(commandList);
    }

private:
    Surface& m_surface;
};

class Surface::SyncCommand : public ICommand {
public:
    explicit SyncCommand(const std::shared_ptr<DualBuffer>& dualBuffer)
        : m_dualBuffer(dualBuffer)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_dualBuffer->sync(commandList);
    }

private:
    std::shared_ptr<DualBuffer> m_dualBuffer;
};

class Surface::RenderCommand1 : public ICommand {
public:
    explicit RenderCommand1(
        const std::shared_ptr<RenderContext>& renderContext,
        const std::shared_ptr<UniformBuffer>& uniformBuffer,
        const std::shared_ptr<IBuffer>& vertex,
        const std::shared_ptr<IBuffer>& index,
        int32_t indexLength)
        : m_renderContext(renderContext)
        , m_uniformBuffer(uniformBuffer)
        , m_vertex(vertex)
        , m_index(index)
        , m_indexLength(indexLength)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_renderContext->render(commandList, m_uniformBuffer, m_vertex, m_index, m_indexLength);
    }

private:
    std::shared_ptr<RenderContext> m_renderContext;
    std::shared_ptr<UniformBuffer> m_uniformBuffer;
    std::shared_ptr<IBuffer> m_vertex;
    std::shared_ptr<IBuffer> m_index;
    int32_t m_indexLength;
};

class Surface::RenderCommand2 : public ICommand {
public:
    explicit RenderCommand2(
        const std::shared_ptr<RenderContext>& renderContext,
        const std::shared_ptr<UniformBuffer>& uniformBuffer,
        const std::shared_ptr<IBuffer>& vertex,
        const std::shared_ptr<IBuffer>& index,
        int32_t indexLength,
        const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
        int32_t instanceCount)
        : m_renderContext(renderContext)
        , m_uniformBuffer(uniformBuffer)
        , m_vertex(vertex)
        , m_index(index)
        , m_indexLength(indexLength)
        , m_instanceBuffers(instanceBuffers)
        , m_instanceCount(instanceCount)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_renderContext->render(commandList, m_uniformBuffer, m_vertex, m_index, m_indexLength, m_instanceBuffers, m_instanceCount);
    }

private:
    std::shared_ptr<RenderContext> m_renderContext;
    std::shared_ptr<UniformBuffer> m_uniformBuffer;
    std::shared_ptr<IBuffer> m_vertex;
    std::shared_ptr<IBuffer> m_index;
    int32_t m_indexLength;
    std::vector<std::shared_ptr<IBuffer>> m_instanceBuffers;
    int32_t m_instanceCount;
};

class Surface::RenderCommand3 : public ICommand {
public:
    explicit RenderCommand3(
        Surface& surface,
        const std::shared_ptr<RenderContext>& renderContext,
        const std::shared_ptr<UniformBuffer>& uniformBuffer,
        const std::shared_ptr<IBuffer>& vertex,
        const std::shared_ptr<IBuffer>& index,
        int32_t indexLength,
        const std::vector<std::shared_ptr<IBuffer>>& instanceBuffers,
        int32_t instanceCount,
        int32_t threadGroupCountX,
        int32_t threadGroupCountY,
        int32_t threadGroupCountZ)
        : m_surface(surface)
        , m_renderContext(renderContext)
        , m_uniformBuffer(uniformBuffer)
        , m_vertex(vertex)
        , m_index(index)
        , m_indexLength(indexLength)
        , m_instanceBuffers(instanceBuffers)
        , m_instanceCount(instanceCount)
        , m_threadGroupCountX(threadGroupCountX)
        , m_threadGroupCountY(threadGroupCountY)
        , m_threadGroupCountZ(threadGroupCountZ)
    {
    }

    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) override
    {
        m_uniformBuffer->beginCompute(commandList);
        m_renderContext->compute(commandList, m_uniformBuffer, m_threadGroupCountX, m_threadGroupCountY, m_threadGroupCountZ);
        m_uniformBuffer->syncCompute(commandList);
        m_uniformBuffer->endCompute(commandList);

        m_surface.render(m_renderContext, m_uniformBuffer, m_vertex, m_index, m_indexLength, m_instanceBuffers, m_instanceCount);
    }

private:
    Surface& m_surface;
    std::shared_ptr<RenderContext> m_renderContext;
    std::shared_ptr<UniformBuffer> m_uniformBuffer;
    std::shared_ptr<IBuffer> m_vertex;
    std::shared_ptr<IBuffer> m_index;
    int32_t m_indexLength;
    std::vector<std::shared_ptr<IBuffer>> m_instanceBuffers;
    int32_t m_instanceCount;
    int32_t m_threadGroupCountX;
    int32_t m_threadGroupCountY;
    int32_t m_threadGroupCountZ;
};
// Impl
class Surface::Impl {
public:
    explicit Impl()
        : queue()
    {
    }
    Utils::BlockingQueue<std::shared_ptr<ICommand>> queue;
};
// public
Surface::~Surface()
{
}

void Surface::beginGui()
{
    m_impl->queue.enqueue(std::make_shared<BeginGuiCommand>(m_swapchain));
}

void Surface::endGui()
{
    m_impl->queue.enqueue(std::make_shared<EndGuiCommand>(m_swapchain));
}

void Surface::begin3D()
{
    m_impl->queue.enqueue(std::make_shared<Begin3DCommand>(*this));
}

void Surface::end3D()
{
    m_impl->queue.enqueue(std::make_shared<End3DCommand>(*this));
}

void Surface::begin2D()
{
    m_impl->queue.enqueue(std::make_shared<Begin2DCommand>(*this));
}

void Surface::end2D()
{
    m_impl->queue.enqueue(std::make_shared<End2DCommand>(*this));
}

void Surface::present()
{
    m_swapchain->present();
    m_swapchain->waitSync();

    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator.Get(), nullptr);
    UniformPool::releaseAll();
}

void Surface::sync(const std::shared_ptr<DualBuffer>& dualBuffer)
{
    m_impl->queue.enqueue(std::make_shared<SyncCommand>(dualBuffer));
}

void Surface::render(
    const std::shared_ptr<RenderContext>& rc,
    const std::shared_ptr<UniformBuffer>& ub,
    const std::shared_ptr<IBuffer>& vertex,
    const std::shared_ptr<IBuffer>& index,
    int32_t indexLength)
{
    m_impl->queue.enqueue(std::make_shared<RenderCommand1>(rc, ub, vertex, index, indexLength));
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
    m_impl->queue.enqueue(std::make_shared<RenderCommand2>(rc, ub, vertex, index, indexLength, instanceBuffers, instanceCount));
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
    m_impl->queue.enqueue(std::make_shared<RenderCommand3>(*this, rc, ub, vertex, index, indexLength, instanceBuffers, instanceCount, threadGroupCountX, threadGroupCountY, threadGroupCountZ));
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
    GlobalLight::destroy();
    PointLight::destroy();
    BloomEffect::destroy();
    m_swapchain->destroy();
    m_swapchain = nullptr;
}
// private
Surface::Surface()
    : m_impl()
    , m_thread()
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
        texResDesc.Width = Screen::getWidth();
        texResDesc.Height = Screen::getHeight();
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
    viewport.Width = Screen::getWidth();
    viewport.Height = Screen::getHeight();
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    m_commandList->RSSetViewports(1, &viewport);
    // scissor
    D3D12_RECT scissorRect = {};
    scissorRect.top = 0;
    scissorRect.left = 0;
    scissorRect.right = scissorRect.left + Screen::getWidth();
    scissorRect.bottom = scissorRect.top + Screen::getHeight();
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
    while (true) {
        auto cmd = m_impl->queue.dequeue();
        cmd->execute(m_commandList);
    }
}
}