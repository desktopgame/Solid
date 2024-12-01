#include <Common/Util/Time.hpp>
#include <thread>

namespace App::Common::Util {
std::chrono::system_clock::time_point Time::s_start;
std::chrono::system_clock::time_point Time::s_end;
// public

void Time::begin() { s_start = std::chrono::system_clock::now(); }
void Time::end() { s_end = std::chrono::system_clock::now(); }
void Time::sync()
{
    int32_t step = 1000.0f / 60.0f;
    int32_t dt = deltaTime() * 1000.0f;

    if (dt < step) {
        int32_t remine = (step - dt);
        std::this_thread::sleep_for(std::chrono::milliseconds(remine));
    }
}

float Time::deltaTime()
{
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(s_end - s_start);
    return static_cast<float>(duration.count() / 1000);
}
}