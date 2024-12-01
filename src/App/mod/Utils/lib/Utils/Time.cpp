#include <Utils/Time.hpp>
#include <thread>

namespace Lib::Utils {
std::chrono::system_clock::time_point Time::s_start;
std::chrono::system_clock::time_point Time::s_end;
float Time::s_deltaTime;
// public

void Time::begin() { s_start = std::chrono::system_clock::now(); }
void Time::end()
{
    s_end = std::chrono::system_clock::now();

    auto elapsedTime = std::chrono::duration<float>(s_end - s_start);
    s_deltaTime = elapsedTime.count();
}
void Time::sync()
{
    std::chrono::milliseconds frame_duration(1000 / 60);
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(s_end - s_start);
    auto sleepTime = frame_duration - elapsedTime;
    if (sleepTime.count() > 0) {
        std::this_thread::sleep_for(sleepTime);
    }
}

float Time::deltaTime() { return s_deltaTime; }
}