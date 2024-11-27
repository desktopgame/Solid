#include <Graphics.hpp>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

#ifndef TEST_EMBED_DIR
#define TEST_EMBED_DIR "."
#endif

static bool endsWith(const std::string& fullString, const std::string& ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

int main(int argc, char* argv[])
{
    using namespace Lib::Graphics;
    for (const auto& entry : std::filesystem::directory_iterator(TEST_EMBED_DIR)) {
        if (entry.is_directory()) {
            continue;
        }
        std::string filename = entry.path().filename().string();

        std::ifstream ifs(entry.path());
        assert(!ifs.fail());

        std::stringstream buf;
        std::string line;
        while (std::getline(ifs, line)) {
            buf << line << std::endl;
        }

        std::string code = buf.str();
        try {
            if (endsWith(filename, ".vs")) {
                Shader::compile("vs_5_0", "vsMain", code, filename);
            } else if (endsWith(filename, ".ps")) {
                Shader::compile("ps_5_0", "psMain", code, filename);
            } else if (endsWith(filename, ".gs")) {
                Shader::compile("gs_5_0", "gsMain", code, filename);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            ::exit(1);
        }
    }
    return 0;
}