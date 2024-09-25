#pragma once
#include <Math/Vector.hpp>
#include <string>
#include <vector>

namespace Lib::Utils {
namespace IO {
    void serializeTile(std::basic_ostream<char>& output, const std::vector<Math::Vector4>& tiles, float tileSize = 1.0f);

    void serializeTile(const std::string& file, const std::vector<Math::Vector4>& tiles, float tileSize = 1.0f);

    void deserializeTile(std::basic_istream<char>& input, std::vector<Math::Vector4>& dst, float tileSize = 1.0f);

    void deserializeTile(const std::string& file, std::vector<Math::Vector4>& dst, float tileSize = 1.0f);
}
}