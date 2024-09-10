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

            /**
             * Layout:
             * Matrix = 0
             * Texture = 1
             * LightDirection = 2
             */
            LightTexture,

            /**
             * Layout:
             * Matrix = 0
             * Color = 1
             * LightDirection = 2
             */
            LightColor,

            Tile
        };

        Layout() = default;
        inline constexpr Layout(Value value)
            : m_value(value)
        {
        }

        inline constexpr bool operator==(Layout a) const { return m_value == a.m_value; }
        inline constexpr bool operator!=(Layout a) const { return m_value != a.m_value; }

        inline constexpr bool useTexture() const { return m_value == Texture || m_value == TextureAndColor || m_value == LightTexture; }
        inline constexpr bool useColor() const { return m_value == Color || m_value == TextureAndColor || m_value == LightColor; }
        inline constexpr bool useLightDirection() const { return m_value == LightColor || m_value == LightTexture; }

    private:
        Value m_value;
    };

    static std::shared_ptr<Constant> rent(Layout layout);
    static void release();
    ~Constant();

    void update();

    void setModelMatrix(const Math::Matrix& modelMatrix);
    Math::Matrix getModelMatrix() const;

    void setViewMatrix(const Math::Matrix& viewMatrix);
    Math::Matrix getViewMatrix() const;

    void setProjectionMatrix(const Math::Matrix& projectionMatrix);
    Math::Matrix getProjectionMatrix() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Texture> getTexture() const;

    void setColor(const Math::Vector4& color);
    Math::Vector4 getColor() const;

    void setLightDirection(const Math::Vector3& lightDirection);
    Math::Vector3 getLightDirection() const;

    Layout getLayout() const;

#if SOLID_ENABLE_INTERNAL
    static void destroy();

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> getID3D12DescriptorHeap() const;
#endif

private:
    Constant(Layout layout);
    void defineConstant(uint64_t width);
    void defineConstantView(int32_t constantIndex, int32_t slotIndex);
    void defineTextureView(const std::shared_ptr<Texture>& texture, int32_t slotIndex);

    static std::shared_ptr<std::vector<std::shared_ptr<Constant>>> s_freeVec;
    static std::shared_ptr<std::vector<std::shared_ptr<Constant>>> s_usedVec;

    bool m_isDirty;
    Math::Matrix m_modelMatrix;
    Math::Matrix m_viewMatrix;
    Math::Matrix m_projectionMatrix;
    std::shared_ptr<Texture> m_texture;
    Math::Vector4 m_color;
    Math::Vector3 m_lightDirection;
    Layout m_layout;

#if SOLID_ENABLE_INTERNAL
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_resources;
#endif
};
}