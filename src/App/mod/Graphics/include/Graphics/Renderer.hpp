#pragma once
#include <Graphics/Color.hpp>
#include <Graphics/FontMap.hpp>
#include <Graphics/TileBuffer.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <memory>

namespace Lib::Graphics {
class Buffer;
class Texture;
class TileBatch;
class RenderContext;
class UniformBuffer;
class Renderer {
public:
    enum class TextAlignX {
        Center = 0,
        Left,
        Right
    };
    enum class TextAlignY {
        Center = 0,
        Top,
        Bottom
    };

    explicit Renderer();

    void textFont(const std::shared_ptr<FontMap>& fontMap);
    void textFontSize(int32_t fontSize);

    void drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color);
    void drawCircle(const Math::Vector2& position, const Math::Vector2& size, const Color& color);
    void drawSprite(const Math::Vector2& position, const Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color);
    void drawText(const Math::Vector2& position, TextAlignX alignX, TextAlignY alignY, const std::u16string& label, const Color& color);
    Math::Vector2 measureText(const std::u16string& label, TextAlignY alignY);

    void drawPlane(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const Math::Vector4& color, bool isWireframe);
    void drawPlaneTexture(const Math::Vector3& position, const Math::Vector2& scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color);

    void drawBox(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const Math::Vector4& color);
    void drawBoxTexture(const Math::Vector3& position, const Math::Vector3& scale, const Math::Quaternion& rotation, const std::shared_ptr<Texture>& texture, const Math::Vector4& color);

private:
    class Object {
    public:
        explicit Object() = default;
        std::shared_ptr<RenderContext> rc;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
        int32_t indexLength;
    };

    void initRect();
    void initCircle();
    void initSprite();
    void initText();

    void initPlane(Object& dst, bool isWireframe);
    void initPlaneTexture();
    void initBox(Object& dst, bool isWireframe);
    void initBoxTexture();

    void renderObject(const Object& object, const std::shared_ptr<UniformBuffer>& ub);

    // text
    std::shared_ptr<FontMap> m_fontMap;
    int32_t m_fontSize;

    Object m_rectObject;
    Object m_circleObject;
    Object m_spriteObject;
    Object m_textObject;
    Object m_planeObject;
    Object m_planeWireframeObject;
    Object m_planeTextureObject;
    Object m_boxObject;
    Object m_boxWireframeObject;
    Object m_boxTextureObject;
};
}
