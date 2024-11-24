#pragma once
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <vector>

namespace Lib::Graphics::Reflect {
enum class InputLayout {
    Vertex2D,
    VertexTexCoord2D,
    Vertex3D,
    VertexNormal3D,
    VertexTexCoord3D,
    VertexNormalTexCoord3D
};

enum class InstanceBufferType {
    Vector2,
    Vector3,
    Vector4,
};

enum class PrimitiveType {
    Polygon,
    Triangles,
    Lines,
    LineStrip,
};

class UCamera {
public:
    Math::Matrix modelMatrix;
    Math::Matrix viewMatrix;
    Math::Matrix projectionMatrix;
    std::array<Math::Vector4, 4> padding;
};
static_assert(sizeof(UCamera) == 256);

class UFloat {
public:
    float value;
    std::array<float, 63> padding;
};
static_assert(sizeof(UFloat) == 256);

class UVector2 {
public:
    Math::Vector2 value;
    std::array<float, 62> padding;
};
static_assert(sizeof(UVector2) == 256);

class UVector3 {
public:
    Math::Vector3 value;
    std::array<float, 61> padding;
};
static_assert(sizeof(UVector3) == 256);

class UVector4 {
public:
    Math::Vector4 value;
    std::array<float, 60> padding;
};
static_assert(sizeof(UVector4) == 256);

class UTileTransform {
public:
    std::array<Math::Matrix, 6> translateMatrixTable;
    std::array<Math::Matrix, 6> rotationMatrixTable;
    std::array<Math::Vector3, 6> normalVectorTable;
    std::array<float, 46> padding;
};
static_assert(sizeof(UTileTransform) == 1024);

class UTilePallet {
public:
    std::array<Math::Vector4, 64> table;
};
static_assert(sizeof(UTilePallet) == 1024);
}