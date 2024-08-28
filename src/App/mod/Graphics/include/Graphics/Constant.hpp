#pragma once
#include <Graphics/Surface.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <memory>
#include <vector>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Shader;
class Buffer;
class Texture;
class Constant {
public:
    class Layout {
    public:
        enum Value : uint8_t {
            None = 0,
            /**
             * Layout:
             * Matrix = 0
             * Texture = 1
             */
            Texture,

            /**
             * Layout:
             * Matrix = 0
             * Color = 1
             */
            Color,

            /**
             * Layout:
             * Matrix = 0
             * Texture = 1
             * Color = 2
             */
            TextureAndColor,
        };

        Layout() = default;
        inline constexpr Layout(Value value)
            : m_value(value)
        {
        }

        inline constexpr bool operator==(Layout a) const { return m_value == a.m_value; }
        inline constexpr bool operator!=(Layout a) const { return m_value != a.m_value; }

        inline constexpr bool useTexture() const { return m_value == Texture || m_value == TextureAndColor; }
        inline constexpr bool useColor() const { return m_value == Color || m_value == TextureAndColor; }

    private:
        Value m_value;
    };

    inline static constexpr int32_t CbMatrixIndex = 0;
    inline static constexpr int32_t CbColorIndex = 1;

    static std::shared_ptr<Constant> rent(Layout layout);
    static void release();
    ~Constant();

    void update();

    void setTransform(const Math::Matrix& transform);
    Math::Matrix getTransform() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> getTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;

    Layout getLayout() const;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getID3D12DescriptorHeap() const;
#endif

private:
    Constant(Layout layout);
    void defineConstant(uint64_t width);
    void defineConstantView(int32_t constantIndex, int32_t slotIndex);
    void defineTextureView(const std::shared_ptr<Texture>& texture, int32_t slotIndex);

    static std::vector<std::shared_ptr<Constant>> s_freeVec;
    static std::vector<std::shared_ptr<Constant>> s_usedVec;

    bool m_isDirty;
    Math::Matrix m_transform;
    std::shared_ptr<Texture> m_texture;
    Math::Vector4 m_color;
    Layout m_layout;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_resources;
#endif
};
}