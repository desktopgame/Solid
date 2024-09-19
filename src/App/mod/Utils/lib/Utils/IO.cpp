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
        std::stringstream buf;
        int32_t mode = 0;
        while (std::getline(input, line)) {
            int32_t at = 0;
            for (char c : line) {
                if (mode == 0) {
                    if (::isdigit(c) || c == '.') {
                        buf << c;
                    } else if (c == ',') {
                        a.at(at) = std::stof(buf.str());
                        buf.str("");
                        buf.clear(std::stringstream::goodbit);
                        mode = 1;
                    }
                } else if (mode == 1) {
                    if (::isspace(c)) {
                        // ok
                    } else if (::isdigit(c)) {
                        buf << c;
                        at++;
                        mode = 0;
                    }
                }
            }
            assert(at == 3);
            a.at(at) = std::stof(buf.str());
            buf.str("");
            buf.clear(std::stringstream::goodbit);
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