#pragma once
#include <chrono>

namespace Lib::Utils {
class Time {
public:
    static void begin();
    static void end();
    static void sync();

    static float deltaTime();

    static float s_deltaTime;

private:
    Time() = delete;
    static std::chrono::system_clock::time_point s_start;
    static std::chrono::system_clock::time_point s_end;
    static std::chrono::milliseconds s_overSleep;
};
}