#pragma once
#include <library.hpp>

namespace App::Common::Graphics {
class Telop;
class TelopSystem {
public:
    static void initialize();

    static void request(const std::string& text, const Vector3& position, const Vector3& color, float duration);

    static void draw();

    static void stop();

    static void destroy();

private:
    TelopSystem() = delete;

    static std::vector<std::shared_ptr<Telop>> s_freeTable;
    static std::vector<std::shared_ptr<Telop>> s_usedTable;
};
}