#pragma once
#include <Graphics/VertexNormal3D.hpp>
#include <Graphics/VertexNormalTexCoord3D.hpp>
#include <Graphics/VertexTexCoord2D.hpp>
#include <Graphics/VertexTexCoord3D.hpp>
#include <Math/Vector.hpp>
#include <vector>

namespace Lib::Graphics {
class Polygon {
public:
    static void generateRect(std::vector<Math::Vector2>& vertices, std::vector<uint32_t>& indices);
    static void generateRect(std::vector<VertexTexCoord2D>& vertices, std::vector<uint32_t>& indices);

    static void generateCircle(std::vector<Math::Vector2>& vertices, std::vector<uint32_t>& indices);

    static void generatePlane(std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices);
    static void generatePlane(std::vector<VertexNormal3D>& vertices, std::vector<uint32_t>& indices);
    static void generatePlane(std::vector<VertexNormalTexCoord3D>& vertices, std::vector<uint32_t>& indices);

    static void generateBox(std::vector<Math::Vector3>& vertices, std::vector<uint32_t>& indices);
    static void generateBox(std::vector<VertexNormal3D>& vertices, std::vector<uint32_t>& indices);
    static void generateBox(std::vector<VertexNormalTexCoord3D>& vertices, std::vector<uint32_t>& indices);

private:
    Polygon() = delete;
};
}