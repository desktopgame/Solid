#pragma once
#include <Graphics/Device.hpp>
#include <memory>
#include <wrl/client.h>

class IDXGIFactory6;
class ID3D12Device;
class ID3D12GraphicsCommandList;

namespace Lib::Graphics {
class Window;
class Device;
}
namespace Lib::Graphics::Internal {
class Swapchain {
public:
    ~Swapchain();

    void clear(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void execute(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void present(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList);
    void waitSync();

private:
    Swapchain();

    uint64_t m_fenceVal;

    class Impl;
    std::shared_ptr<Impl> m_impl;

    static std::shared_ptr<Swapchain> create(
        const std::shared_ptr<Window>& window,
        const Microsoft::WRL::ComPtr<IDXGIFactory6>& dxgiFactory,
        const Microsoft::WRL::ComPtr<ID3D12Device>& device);
    // friend std::shared_ptr<Device> Device::create(const std::shared_ptr<Window>& window);;
    friend class Lib::Graphics::Device;
};
}