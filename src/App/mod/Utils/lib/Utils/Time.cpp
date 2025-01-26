#include <Utils/Clock.hpp>
#include <Utils/Time.hpp>
#include <Windows.h>
#include <thread>

namespace Lib::Utils {
float Time::s_start;
float Time::s_end;
float Time::s_overSleep;
float Time::s_deltaTime;
// public

void Time::begin() { s_start = Clock::counter(); }
void Time::end()
{
    s_end = Clock::counter();

    s_deltaTime = (s_end - s_start);

    if (s_deltaTime > 1.0f) {
        s_deltaTime = 1.0f;
    }
}
void Time::sync()
{
    float frameDuration = (1.0f / 60.0f);
    float elapsedTime = s_end - s_start;
    float sleepTime = frameDuration - elapsedTime - s_overSleep;
    if (sleepTime > 0) {
        // NOTE: スワップチェインがリフレッシュレートに合わせて同期するのでそちらに任せる
        // ::Sleep((DWORD)(sleepTime * 1000.0f));
        s_overSleep = (Clock::counter() - s_end) - sleepTime;
    } else {
        s_overSleep = 0;
    }
}

float Time::deltaTime() { return s_deltaTime; }
}