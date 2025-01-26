#pragma once

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
    static float s_start;
    static float s_end;
    static float s_overSleep;
};
}