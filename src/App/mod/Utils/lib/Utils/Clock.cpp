#include <Utils/Clock.hpp>

namespace Lib::Utils {
LARGE_INTEGER Clock::s_freq;
LARGE_INTEGER Clock::s_counter;
int64_t Clock::s_start;
double Clock::s_pcFreq;

float Clock::counter()
{
    QueryPerformanceCounter(&s_counter);
    return (float)(double(s_counter.QuadPart - s_start) / s_pcFreq);
}

void Clock::initialize()
{
    QueryPerformanceFrequency(&s_freq);
    s_pcFreq = s_freq.QuadPart;

    QueryPerformanceCounter(&s_counter);
    s_start = s_counter.QuadPart;
}
void Clock::destroy() { }
}