#pragma once
#include <chrono>

namespace App::Common::Util {
class Time {
public:
    static void begin();
    static void end();
    static void sync();

    static float deltaTime();

private:
    Time() = delete;
    static std::chrono::system_clock::time_point s_start;
    static std::chrono::system_clock::time_point s_end;
    static float s_deltaTime;
};
}