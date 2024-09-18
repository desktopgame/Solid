#pragma once
#include <Graphics/Color.hpp>
#include <Graphics/FontMap.hpp>
#include <Graphics/TileBuffer.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <memory>

namespace Lib::Graphics {
class PipelineStateObject;
class Buffer;
class Constant;
class Texture;
class TileBatch;
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

    void position(const Math::Vector3& position);
    void lookAt(const Math::Vector3& lookAt);
    void depthRange(float zNear, float zFar);
    void fovY(float fovY);

    void lightEnable();
    void lightDisable();
    void lightDirection(const Math::Vector3& lightDirection);

    void textFont(const std::shared_ptr<FontMap>& fontMap);
    void textFontSize(int32_t fontSize);

    void guiBegin();
    void guiEnd();

    void begin();
    void end();

    void drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color);
    void drawCircle(const Math::Vector2& position, const Math::Vector2& size, const Color& color);
    void drawSprite(const Math::Vector2& position, const Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color);
    void drawText(const Math::Vector2& position, TextAlignX alignX, TextAlignY alignY, const std::u16string& label, const Color& color);
    Math::Vector2 measureText(const std::u16string& label, TextAlignY alignY);

    void drawPlane(const Math::Vector3& position, const Math::Vector3& size, const Math::Quaternion& rotation, const Color& color);
    void drawBox(const Math::Vector3& position, const Math::Vector3& size, const Math::Quaternion& rotation, const Color& color);

    int32_t rentTile(TileBufferKind kind);
    void releaseTile(TileBufferKind kind, int32_t index);
    void batchTiles(TileBufferKind kind, int32_t index, const Math::Vector4* tiles, int32_t tileCount);
    void batchMatrix(TileBufferKind kind, int32_t index, const Math::Matrix& matrix);
    void drawTiles();

private:
    class Object {
    public:
        explicit Object() = default;
        std::shared_ptr<PipelineStateObject> pso;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
        int32_t indexLength;
    };

    void initRect();
    void initCircle();
    void initSprite();
    void initText();

    void initPlane();
    void initPlaneLighting();
    void initBox();
    void initBoxLighting();

    std::shared_ptr<TileBatch> getTileBatch(TileBufferKind kind);

    void renderObject(const Object& object, const std::shared_ptr<Constant> constant);
    Math::Matrix getOrthoMatrix();
    Math::Matrix getLookAtMatrix();
    Math::Matrix getPerspectiveMatrix();

    // camera
    Math::Vector3 m_position;
    Math::Vector3 m_lookAt;
    float m_zNear;
    float m_zFar;
    float m_fovY;
    // light
    bool m_lightEnable;
    Math::Vector3 m_lightDirection;
    // text
    std::shared_ptr<FontMap> m_fontMap;
    int32_t m_fontSize;

    bool m_dirtyOrthoMatrix;
    bool m_dirtyViewMatrix;
    bool m_dirtyProjectionMatrix;
    Math::Matrix m_orthoMatrix;
    Math::Matrix m_viewMatrix;
    Math::Matrix m_projectionMatrix;

    Object m_rectObject;
    Object m_circleObject;
    Object m_spriteObject;
    Object m_textObject;

    Object m_planeObject;
    Object m_planeLightingObject;
    Object m_boxObject;
    Object m_boxLightingObject;

    std::array<std::shared_ptr<TileBatch>, 5> m_tileBatches;
};
}