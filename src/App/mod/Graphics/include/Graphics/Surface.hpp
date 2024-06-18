#pragma once
#include <Graphics/Device.hpp>
#include <Graphics/PrimitiveType.hpp>
#include <any>
#include <memory>
#include <vector>

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
class RenderContext;
class Surface {
public:
    ~Surface();
    void begin();
    void end();

    void render(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<RenderParameter>& renderParameter,
        PrimitiveType primitiveType,
        int32_t vertexComponent,
        bool isUsingTexCoord,
        const std::shared_ptr<Buffer>& vertexBuffer,
        const std::shared_ptr<Buffer>& indexBuffer,
        int32_t indexLength);

    void render(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<RenderParameter>& renderParameter,
        const std::shared_ptr<RenderContext>& context);

private:
    Surface();

    std::shared_ptr<Internal::Swapchain> m_swapchain;

    class PsoHash;
    std::vector<std::shared_ptr<PsoHash>> m_psoTable;

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