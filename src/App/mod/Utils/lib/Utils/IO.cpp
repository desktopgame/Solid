#include <Utils/IO.hpp>
#include <cassert>
#include <cctype>
#include <fstream>
#include <sstream>

namespace Lib::Utils {
namespace IO {

    void serializeTile(std::basic_ostream<char>& output, const std::vector<Math::Vector4>& tiles)
    {
        for (auto& tile : tiles) {
            output << tile.x() << "," << tile.y() << "," << tile.z() << "," << tile.w() << std::endl;
        }
        output.flush();
    }

    void serializeTile(const std::string& file, const std::vector<Math::Vector4>& tiles)
    {
        std::ofstream ofs(file);
        if (ofs.fail()) {
            return;
        }
        serializeTile(ofs, tiles);
    }

    void deserializeTile(std::basic_istream<char>& input, std::vector<Math::Vector4>& dst)
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

            dst.push_back(Math::Vector4(a));
        }
    }

    void deserializeTile(const std::string& file, std::vector<Math::Vector4>& dst)
    {
        std::ifstream ifs(file);
        if (ifs.fail()) {
            return;
        }
        deserializeTile(ifs, dst);
    }
}
}