#pragma once
#include <Graphics/Device.hpp>
#include <Graphics/PrimitiveType.hpp>
#include <any>
#include <memory>

namespace Lib::Graphics::Internal {
class Swapchain;
class Pso;
}

namespace Lib::Graphics {
class Device;
class Window;
class Buffer;
class Shader;
class RenderParameter;
class Surface {
public:
    ~Surface();
    void render();
    void begin();
    void end();

    void draw(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<RenderParameter>& renderParameter,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool isUsingTexCoord,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

private:
    Surface();

    std::shared_ptr<Internal::Swapchain> m_swapchain;
    std::shared_ptr<Internal::Pso> m_pso;

    class Impl;
    std::shared_ptr<Impl> m_impl;

    static std::shared_ptr<Surface> create(
        const std::shared_ptr<Device>& device,
        const std::shared_ptr<Internal::Swapchain>& swapchain);
    void destroy();
    // friend std::shared_ptr<Device> Device::create(const std::shared_ptr<Window>& window);
    friend class Lib::Graphics::Device;
};
}