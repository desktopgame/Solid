#pragma once
#include <Graphics/RenderInterface.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <wrl/client.h>

class ID3D12DescriptorHeap;
class ID3D12Resource;

namespace Lib::Graphics {
class Texture;
}

namespace Lib::Graphics::Internal {
class Constant {
public:
    inline static constexpr int32_t CbMatrixIndex = 0;
    inline static constexpr int32_t CbColorIndex = 1;

    static std::shared_ptr<Constant> create(RenderInterface interfaze);

    void update();

    void setTransform(const Math::Matrix& transform);
    Math::Matrix getTransform() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> getTexture() const;
    bool useTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;
    bool useColor() const;

    RenderInterface getInterface() const;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getDescriptorHeap() const;

private:
    Constant(RenderInterface interfaze);

    void defineConstant(uint64_t width);
    void defineConstantView(int32_t constantIndex, int32_t slotIndex);
    void defineTextureView(const std::shared_ptr<Texture>& texture, int32_t slotIndex);

    bool m_isDirty;
    Math::Matrix m_transform;
    std::shared_ptr<Texture> m_texture;
    Math::Vector4 m_color;
    RenderInterface m_interface;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_resources;
};
}