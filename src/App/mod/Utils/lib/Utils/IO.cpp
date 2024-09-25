#include <Utils/IO.hpp>
#include <cassert>
#include <cctype>
#include <fstream>
#include <sstream>

namespace Lib::Utils {
namespace IO {

    void serializeTile(std::basic_ostream<char>& output, const std::vector<Math::Vector4>& tiles, float tileSize)
    {
        for (auto& tile : tiles) {
            int32_t ix = static_cast<int32_t>(tile.x() / tileSize);
            int32_t iy = static_cast<int32_t>(tile.y() / tileSize);
            int32_t iz = static_cast<int32_t>(tile.z() / tileSize);
            int32_t iw = static_cast<int32_t>(tile.w());
            output << ix << "," << iy << "," << iz << "," << iw << std::endl;
        }
        output.flush();
    }

    void serializeTile(const std::string& file, const std::vector<Math::Vector4>& tiles, float tileSize)
    {
        std::ofstream ofs(file);
        if (ofs.fail()) {
            return;
        }
        serializeTile(ofs, tiles, tileSize);
    }

    void deserializeTile(std::basic_istream<char>& input, std::vector<Math::Vector4>& dst, float tileSize)
    {
        dst.clear();
        std::array<float, 4> a;
        std::string line;
        while (!input.eof()) {
            char tmp;

            input >> a.at(0);
            input >> std::ws;
            input >> tmp;
            input >> std::ws;
            assert(tmp == ',');

            input >> a.at(1);
            input >> std::ws;
            input >> tmp;
            input >> std::ws;
            assert(tmp == ',');

            input >> a.at(2);
            input >> std::ws;
            input >> tmp;
            input >> std::ws;
            assert(tmp == ',');

            input >> a.at(3);
            input >> std::ws;

            Math::Vector4 scale({ tileSize, tileSize, tileSize, 1.0f });
            dst.push_back(Math::Vector4(a) * scale);
        }
    }

    void deserializeTile(const std::string& file, std::vector<Math::Vector4>& dst, float tileSize)
    {
        std::ifstream ifs(file);
        if (ifs.fail()) {
            return;
        }
        deserializeTile(ifs, dst, tileSize);
    }
}
}