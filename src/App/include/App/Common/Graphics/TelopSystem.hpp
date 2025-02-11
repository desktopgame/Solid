#pragma once
#include <App/library.hpp>

namespace App::Common::Graphics {
class Telop;
class TelopSystem {
public:
    static void initialize();

    static void request(const std::u16string& text, const Vector3& position, const Vector3& color, float duration);

    static void draw();

    static void stop();

    static void destroy();

private:
    TelopSystem() = delete;

    static std::vector<std::shared_ptr<Telop>> s_freeTable;
    static std::vector<std::shared_ptr<Telop>> s_usedTable;

    static std::shared_ptr<FontMap> s_fontMap;
    static std::shared_ptr<CpuBuffer> s_vertexBuffer;
    static std::shared_ptr<CpuBuffer> s_indexBuffer;
    static int32_t s_indexLength;
};
}