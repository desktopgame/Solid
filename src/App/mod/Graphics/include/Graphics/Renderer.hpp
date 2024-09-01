#pragma once
#include <Graphics/Color.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::Graphics {
class PipelineStateObject;
class Buffer;
class Constant;
class Texture;
class Renderer {
public:
    explicit Renderer();

    void position(const Math::Vector3& position);
    void lookAt(const Math::Vector3& lookAt);
    void depthRange(float zNear, float zFar);
    void fovY(float fovY);

    void lightEnable();
    void lightDisable();
    void lightDirection(const Math::Vector3& lightDirection);

    void drawRect(const Math::Vector2& position, const Math::Vector2& size, float degree, const Color& color);
    void drawCircle(const Math::Vector2& position, const Math::Vector2& size, const Color& color);
    void drawSprite(const Math::Vector2& position, const Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color);

    void drawPlane(const Math::Vector3& position, const Math::Vector3& size, const Math::Quaternion& rotation, const Color& color);
    void drawBox(const Math::Vector3& position, const Math::Vector3& size, const Math::Quaternion& rotation, const Color& color);

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

    void initPlane();
    void initPlaneLighting();
    void initBox();
    void initBoxLighting();

    void renderObject(const Object& object, const std::shared_ptr<Constant> constant);
    Math::Matrix getOrthoMatrix();
    Math::Matrix getLookAtMatrix();
    Math::Matrix getPerspectiveMatrix();

    Math::Vector3 m_position;
    Math::Vector3 m_lookAt;
    float m_zNear;
    float m_zFar;
    float m_fovY;
    bool m_lightEnable;
    Math::Vector3 m_lightDirection;
    bool m_dirtyOrthoMatrix;
    bool m_dirtyViewMatrix;
    bool m_dirtyProjectionMatrix;
    Math::Matrix m_orthoMatrix;
    Math::Matrix m_viewMatrix;
    Math::Matrix m_projectionMatrix;

    Object m_rectObject;
    Object m_circleObject;
    Object m_spriteObject;

    Object m_planeObject;
    Object m_planeLightingObject;
    Object m_boxObject;
    Object m_boxLightingObject;
};
}