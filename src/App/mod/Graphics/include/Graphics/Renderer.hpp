#pragma once
#include <Graphics/Color.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vector.hpp>
#include <memory>

namespace Lib::Graphics {
class PipelineStateObject;
class Buffer;
class Constant;
class Renderer {
public:
    explicit Renderer();

    void drawRect(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, const Color& color);
    void drawCircle(const Lib::Math::Vector2& position, const Lib::Math::Vector2& size, const Color& color);

    void drawPlane(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Color& color);
    void drawBox(const Lib::Math::Vector3& position, const Lib::Math::Vector3& size, const Color& color);

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

    void initPlane();
    void initBox();

    void renderObject(const Object& object, const std::shared_ptr<Constant> constant);

    Object m_rectObject;
    Object m_circleObject;

    Object m_planeObject;
    Object m_boxObject;
};
}