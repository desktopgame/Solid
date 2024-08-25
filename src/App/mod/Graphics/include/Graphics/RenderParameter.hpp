#pragma once
#include <Graphics/RenderInterface.hpp>
#include <Graphics/Surface.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics::Internal {
class Constant;
}

namespace Lib::Graphics {
class Shader;
class Buffer;
class Texture;
class RenderParameter {
public:
    inline static constexpr int32_t CbMatrixIndex = 0;
    inline static constexpr int32_t CbColorIndex = 1;

    static std::shared_ptr<RenderParameter> create(RenderInterface interfaze);
    ~RenderParameter();

    void update();

    void setTransform(const Math::Matrix& transform);
    Math::Matrix getTransform() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> getTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;

    RenderInterface getInterface() const;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getID3D12DescriptorHeap() const;
#endif

private:
    RenderParameter(RenderInterface interfaze);
    void defineConstant(uint64_t width);
    void defineConstantView(int32_t constantIndex, int32_t slotIndex);
    void defineTextureView(const std::shared_ptr<Texture>& texture, int32_t slotIndex);

#if SOLID_ENABLE_INTERNAL
    bool m_isDirty;
    Math::Matrix m_transform;
    std::shared_ptr<Texture> m_texture;
    Math::Vector4 m_color;
    RenderInterface m_interface;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_resources;
#endif
};
}