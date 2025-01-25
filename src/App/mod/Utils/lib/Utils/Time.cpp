#include <Utils/Time.hpp>
#include <thread>

namespace Lib::Utils {
std::chrono::system_clock::time_point Time::s_start;
std::chrono::system_clock::time_point Time::s_end;
std::chrono::milliseconds Time::s_overSleep;
float Time::s_deltaTime;
// public

void Time::begin() { s_start = std::chrono::system_clock::now(); }
void Time::end()
{
    s_end = std::chrono::system_clock::now();

    auto elapsedTime = std::chrono::duration<float>(s_end - s_start);
    s_deltaTime = elapsedTime.count();

    if (s_deltaTime > 1.0f) {
        s_deltaTime = 1.0f;
    }
}
void Time::sync()
{
    std::chrono::milliseconds frameDuration(1000 / 60);
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(s_end - s_start);
    auto sleepTime = frameDuration - elapsedTime - s_overSleep;
    if (sleepTime.count() > 0) {
        // NOTE: DirectXがリフレッシュレートに合わせて同期するので不要
        // std::this_thread::sleep_for(sleepTime);
        s_overSleep = std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::system_clock::now() - s_end) - sleepTime);
    } else {
        s_overSleep = std::chrono::milliseconds(0);
    }
}

float Time::deltaTime() { return s_deltaTime; }
}