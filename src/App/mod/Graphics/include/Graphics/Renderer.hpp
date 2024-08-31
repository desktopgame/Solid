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

    void position(const Lib::Math::Vector3& position);
    void lookAt(const Lib::Math::Vector3& lookAt);
    void depthRange(float zNear, float zFar);
    void fovY(float fovY);

    void drawRect(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, float degree, const Color& color);
    void drawCircle(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, float degree, const Color& color);
    void drawSprite(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, float degree, const std::shared_ptr<Texture>& texture, const Color& color);

    void drawPlane(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Lib::Math::Quaternion& rotation, const Color& color);
    void drawBox(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Lib::Math::Quaternion& rotation, const Color& color);

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
    Lib::Math::Matrix transform2D(const Lib::Math::Matrix& m);
    Lib::Math::Matrix transform3D(const Lib::Math::Matrix& m);

    Lib::Math::Vector3 m_position;
    Lib::Math::Vector3 m_lookAt;
    float m_zNear;
    float m_zFar;
    float m_fovY;
    bool m_dirtyOrthoMatrix;
    bool m_dirtyViewMatrix;
    bool m_dirtyProjectionMatrix;
    Lib::Math::Matrix m_orthoMatrix;
    Lib::Math::Matrix m_viewMatrix;
    Lib::Math::Matrix m_projectionMatrix;

    Object m_rectObject;
    Object m_circleObject;
    Object m_spriteObject;

    Object m_planeObject;
    Object m_planeLightingObject;
    Object m_boxObject;
    Object m_boxLightingObject;
};
}