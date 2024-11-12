#include <Graphics/Polygon.hpp>

namespace Lib::Graphics {

void Polygon::generateRect(std::vector<Math::Vector2>& vertices, std::vector<uint32_t>& indices)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(Math::Vector2({ left, top }));
    vertices.push_back(Math::Vector2({ right, top }));
    vertices.push_back(Math::Vector2({ right, bottom }));
    vertices.push_back(Math::Vector2({ left, bottom }));

    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(1);
}
void Polygon::generateRect(std::vector<VertexTexCoord2D>& vertices, std::vector<uint32_t>& indices)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, top }), Math::Vector2({ 0.0f, 0.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, top }), Math::Vector2({ 1.0f, 0.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ right, bottom }), Math::Vector2({ 1.0f, 1.0f })));
    vertices.push_back(VertexTexCoord2D(Math::Vector2({ left, bottom }), Math::Vector2({ 0.0f, 1.0f })));

    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(1);
}

void Polygon::generateCircle(std::vector<Math::Vector2>& vertices, std::vector<uint32_t>& indices)
{
    float degree = 0.0f;
    vertices.emplace_back(Math::Vector2({ 0.0f, 0.0f }));
    while (degree < 360.0f) {
        float x = Math::Mathf::cos(Math::Mathf::Deg2Rad * degree) * 0.5f;
        float y = Math::Mathf::sin(Math::Mathf::Deg2Rad * degree) * 0.5f;
        vertices.emplace_back(Math::Vector2({ x, y }));
        degree += 5.0f;
    }

    uint32_t index = 1;
    while (index < vertices.size()) {
        indices.emplace_back(0);
        indices.emplace_back(index + 1);
        indices.emplace_back(index);
        index = index + 1;
    }
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(index - 1);
}

void Polygon::generatePlane(std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(Math::Vector3({ left, bottom, 0 }));
    vertices.push_back(Math::Vector3({ left, top, 0 }));
    vertices.push_back(Math::Vector3({ right, bottom, 0 }));
    vertices.push_back(Math::Vector3({ right, top, 0 }));

    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
}
void Polygon::generatePlane(std::vector<VertexNormal3D>& vertices, std::vector<uint32_t>& indices)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexNormal3D(Math::Vector3({ left, bottom, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ left, top, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ right, bottom, 0 }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ right, top, 0 }), Math::Vector3({ 0, 0, -1 })));

    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
}
void Polygon::generatePlane(std::vector<VertexNormalTexCoord3D>& vertices, std::vector<uint32_t>& indices)
{
    const float left = -0.5;
    const float right = 0.5;
    const float top = 0.5;
    const float bottom = -0.5;
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ left, bottom, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ left, top, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ right, bottom, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ right, top, 0 }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 1, 0 })));

    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(3);
}

void Polygon::generateBox(std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices)
{
    // z-
    vertices.push_back(Math::Vector3({ -0.5f, -0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, 0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, -0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, 0.5f, -0.5f }));
    // z+
    vertices.push_back(Math::Vector3({ 0.5f, -0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, 0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, -0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, 0.5f, 0.5f }));
    // x-
    vertices.push_back(Math::Vector3({ -0.5f, -0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, 0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, -0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, 0.5f, -0.5f }));
    // x+
    vertices.push_back(Math::Vector3({ 0.5f, -0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, 0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, -0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, 0.5f, 0.5f }));
    // y-
    vertices.push_back(Math::Vector3({ -0.5f, -0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, -0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, -0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, -0.5f, -0.5f }));
    // y+
    vertices.push_back(Math::Vector3({ -0.5f, 0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ -0.5f, 0.5f, 0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, 0.5f, -0.5f }));
    vertices.push_back(Math::Vector3({ 0.5f, 0.5f, 0.5f }));

    uint32_t offset = 0;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 4;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 8;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 12;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 16;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 20;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
}
void Polygon::generateBox(std::vector<VertexNormal3D>& vertices, std::vector<uint32_t>& indices)
{
    // z-
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 })));
    // z+
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 })));
    // x-
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, -0.5f, 0.5f }), Math::Vector3({ -1, 0, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, 0.5f, 0.5f }), Math::Vector3({ -1, 0, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, -0.5f, -0.5f }), Math::Vector3({ -1, 0, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, 0.5f, -0.5f }), Math::Vector3({ -1, 0, 0 })));
    // x+
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, -0.5f, -0.5f }), Math::Vector3({ 1, 0, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, 0.5f, -0.5f }), Math::Vector3({ 1, 0, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, -0.5f, 0.5f }), Math::Vector3({ 1, 0, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, 0.5f, 0.5f }), Math::Vector3({ 1, 0, 0 })));
    // y-
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, -1, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, -1, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, -1, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, -1, 0 })));
    // y+
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 1, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ -0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 1, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 1, 0 })));
    vertices.push_back(VertexNormal3D(Math::Vector3({ 0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 1, 0 })));

    uint32_t offset = 0;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 4;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 8;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 12;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 16;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 20;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
}
void Polygon::generateBox(std::vector<VertexNormalTexCoord3D>& vertices, std::vector<uint32_t>& indices)
{
    // z-
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 0, -1 }), Math::Vector2({ 1, 0 })));
    // z+
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 0, 1 }), Math::Vector2({ 1, 0 })));
    // x-
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, -0.5f, 0.5f }), Math::Vector3({ -1, 0, 0 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, 0.5f, 0.5f }), Math::Vector3({ -1, 0, 0 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, -0.5f, -0.5f }), Math::Vector3({ -1, 0, 0 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, 0.5f, -0.5f }), Math::Vector3({ -1, 0, 0 }), Math::Vector2({ 1, 0 })));
    // x+
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, -0.5f, -0.5f }), Math::Vector3({ 1, 0, 0 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, 0.5f, -0.5f }), Math::Vector3({ 1, 0, 0 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, -0.5f, 0.5f }), Math::Vector3({ 1, 0, 0 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, 0.5f, 0.5f }), Math::Vector3({ 1, 0, 0 }), Math::Vector2({ 1, 0 })));
    // y-
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, -1, 0 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, -1, 0 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, -0.5f, 0.5f }), Math::Vector3({ 0, -1, 0 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, -0.5f, -0.5f }), Math::Vector3({ 0, -1, 0 }), Math::Vector2({ 1, 0 })));
    // y+
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 1, 0 }), Math::Vector2({ 0, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ -0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 1, 0 }), Math::Vector2({ 0, 0 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, 0.5f, -0.5f }), Math::Vector3({ 0, 1, 0 }), Math::Vector2({ 1, 1 })));
    vertices.push_back(VertexNormalTexCoord3D(Math::Vector3({ 0.5f, 0.5f, 0.5f }), Math::Vector3({ 0, 1, 0 }), Math::Vector2({ 1, 0 })));

    uint32_t offset = 0;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 4;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 8;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 12;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 16;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
    offset = 20;
    indices.emplace_back(offset + 0);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 3);
}

}